/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
**
** "You can get credit for something or get it done, but not both."
** Dr. Richard Garwin
**
** BSD License follows.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** Redistributions of source code must retain the above copyright notice,
** this list of conditions and the following disclaimer. Redistributions
** in binary form must reproduce the above copyright notice, this list
** of conditions and the following disclaimer in the documentation and/or
** other materials provided with the distribution. Neither the name of
** the WFC nor the names of its contributors may be used to endorse or
** promote products derived from this software without specific prior
** written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** $Workfile: CQueue.hpp $
** $Revision: 22 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined ( QUEUE_CLASS_HEADER )

#define QUEUE_CLASS_HEADER

#if ! defined( WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS )
#define WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS (2048)
#endif // WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS

#if ! defined( CQUEUE_MAX_GROW_SIZE )
#define CQUEUE_MAX_GROW_SIZE (131072)
#endif // CQUEUE_MAX_GROW_SIZE

class CQueue
{
protected:

    // What we want to protect

    CRITICAL_SECTION m_AddCriticalSection;
    CRITICAL_SECTION m_GetCriticalSection;

    void * * m_Items{ nullptr };

    std::size_t m_AddIndex{ 0 };
    std::size_t m_GetIndex{ 0 };
    std::size_t m_Size{ 0 };

    HANDLE m_Heap{ INVALID_HANDLE_VALUE };

    inline void m_GrowBy(_In_ std::size_t const number_of_new_items) noexcept;

public:

    CQueue(CQueue const&) = delete;
    CQueue& operator = (CQueue const&) = delete;

    inline  CQueue(_In_ std::size_t initial_size = WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS) noexcept;
    inline ~CQueue();

    inline _Check_return_ bool Add(_In_ std::size_t new_item) noexcept { return(Add((void *)new_item)); };
    inline _Check_return_ bool Add(_In_ void * new_item) noexcept;
    inline void Empty(void) noexcept
    {
        ::EnterCriticalSection(&m_AddCriticalSection);
        ::EnterCriticalSection(&m_GetCriticalSection);
        m_AddIndex = 0;
        m_GetIndex = 0;
        ::LeaveCriticalSection(&m_GetCriticalSection);
        ::LeaveCriticalSection(&m_AddCriticalSection);
    };
    inline _Check_return_ bool  Get(_Out_ std::size_t& item) noexcept { return(Get(reinterpret_cast<void *&>(item))); };
    inline _Check_return_ bool  Get(_Out_ void * & item) noexcept;
    inline _Check_return_ bool  TryGet(_Out_ std::size_t& item) noexcept { return(TryGet(reinterpret_cast<void *&>(item))); };
    inline _Check_return_ bool  TryGet(_Out_ void * & item) noexcept;
    inline _Check_return_ std::size_t GetLength(void) const noexcept;
    inline constexpr _Check_return_ std::size_t GetMaximumLength(void) const noexcept { return(m_Size); };

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

    virtual void Dump(CDumpContext& dump_context) const;

#endif // _DEBUG
};

inline CQueue::CQueue(_In_ std::size_t initial_size) noexcept
{
    // TryEnterCriticalSection is snapped to NTDLL.RtlTryEnterCriticalSection but we
    // will stick to the published specification. We go to this trouble so programs
    // that were compiled for NT4 will work with NT3

    m_AddIndex = 0;
    m_GetIndex = 0;
    m_Items = nullptr;

    if (initial_size == 0)
    {
        initial_size = 1;
    }

    /*
    ** 1999-11-05
    ** We create our own heap because all of the pointers used are allocated
    ** and freed by us. We don't have to worry about a non-serialized thread
    ** accessing something we allocated. Because of this, we can perform our
    ** memory allocations in a heap dedicated to queueing. This means when we
    ** have to allocate more memory, we don't have to wait for all other threads
    ** to pause while we allocate from the shared heap (like the C Runtime heap)
    */

    m_Heap = ::HeapCreate(HEAP_NO_SERIALIZE,
        (((2 * initial_size * sizeof(void *)) < 65536) ? 65536 : (2 * initial_size * sizeof(void *))),
        0);

    if (m_Heap != nullptr)
    {
        m_Items = static_cast<void **>(::HeapAlloc(m_Heap, HEAP_NO_SERIALIZE, initial_size * sizeof(void *)));

        m_Size = (m_Items == nullptr) ? 0 : initial_size;
    }
    else
    {
        m_Items = nullptr;
        m_Size = 0;
    }

    ::InitializeCriticalSection(&m_AddCriticalSection);
    ::InitializeCriticalSection(&m_GetCriticalSection);
}

inline CQueue::~CQueue() noexcept
{
    m_AddIndex = 0;
    m_GetIndex = 0;
    m_Size = 0;

    if (m_Items != nullptr)
    {
        ::HeapFree(m_Heap, HEAP_NO_SERIALIZE, m_Items);
        m_Items = nullptr;
    }

    ::HeapDestroy(m_Heap);
    m_Heap = nullptr;

    ::DeleteCriticalSection(&m_AddCriticalSection);
    ::DeleteCriticalSection(&m_GetCriticalSection);
}

inline _Check_return_ bool CQueue::Add(_In_ void * item) noexcept
{
    // Block other threads from entering Add();
    ::EnterCriticalSection(&m_AddCriticalSection);

    // Add the item

    m_Items[m_AddIndex] = item;

    // 1999-12-08
    // Many many thanks go to Lou Franco (lfranco@spheresoft.com)
    // for finding an bug here. In rare but recreatable situations,
    // m_AddIndex could be in an invalid state.

    // Make sure m_AddIndex is never invalid

    std::size_t new_add_index = ((m_AddIndex + 1) >= m_Size) ? 0 : m_AddIndex + 1;

    if (new_add_index == m_GetIndex)
    {
        // The queue is full. We need to grow.
        // Stop anyone from getting from the queue
        ::EnterCriticalSection(&m_GetCriticalSection);

        m_AddIndex = new_add_index;

        // One last double-check.

        if (m_AddIndex == m_GetIndex)
        {
            m_GrowBy(m_Size);
        }

        ::LeaveCriticalSection(&m_GetCriticalSection);
    }
    else
    {
        m_AddIndex = new_add_index;
    }

    // Let other threads call Add() now.
    ::LeaveCriticalSection(&m_AddCriticalSection);

    return(true);
}

inline _Check_return_ bool CQueue::TryGet(_Out_ void * & item) noexcept
{
    item = nullptr;

    // Prevent other threads from entering Get()
    if (TryEnterCriticalSection(&m_GetCriticalSection) == 0)
    {
        return(false);
    }

    if (m_GetIndex == m_AddIndex)
    {
        // Let's check to see if our queue has grown too big
        // If it has, then shrink it

        if (m_Size > WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS)
        {
            // Yup, we're too big for our britches

            if (TryEnterCriticalSection(&m_AddCriticalSection) != 0)
            {
                // Now, no one can add to the queue

                if (m_GetIndex == m_AddIndex)
                {
                    // See if we can just shrink it... It is safe to use HeapReAlloc() because the queue is empty
                    auto return_value = ::HeapReAlloc(m_Heap, HEAP_NO_SERIALIZE, m_Items, WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS * sizeof(void *));

                    if (return_value != nullptr)
                    {
                        m_Items = static_cast<void **>(return_value);
                    }
                    else
                    {
                        // Looks like we'll have to do it the hard way
                        ::HeapFree(m_Heap, HEAP_NO_SERIALIZE, m_Items);
                        m_Items = static_cast<void **>(::HeapAlloc(m_Heap, HEAP_NO_SERIALIZE, WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS * sizeof(void *)));
                    }

                    m_Size = (m_Items == nullptr) ? 0 : WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS;
                    m_AddIndex = 0;
                    m_GetIndex = 0;
                }
                else
                {
                    // m_GetIndex != m_AddIndex, this means that someone added
                    // to the queue between the time we checked m_Size for being
                    // too big and the time we entered the add critical section.
                    // If this happened then we are too busy to shrink
                }

                // Let people add to the queue once again

                ::LeaveCriticalSection(&m_AddCriticalSection);
            }
        }

        // Let other threads call Get() now, we are empty
        ::LeaveCriticalSection(&m_GetCriticalSection);
        return(false);
    }

    item = m_Items[m_GetIndex];

    // Make sure m_GetIndex is never invalid

    m_GetIndex = ((m_GetIndex + 1) >= m_Size) ? 0 : m_GetIndex + 1;

    // Let other threads call Get() now
    ::LeaveCriticalSection(&m_GetCriticalSection);

    return(true);
}

inline _Check_return_ bool CQueue::Get(_Out_ void * & item) noexcept
{
    item = nullptr;

    // Prevent other threads from entering Get()
    ::EnterCriticalSection(&m_GetCriticalSection);

    if (m_GetIndex == m_AddIndex)
    {
        // TryEnterCriticalSection() is available in NT 4.0 and up

        // Let's check to see if our queue has grown too big
        // If it has, then shrink it

        if (m_Size > WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS)
        {
            // Yup, we're too big for our britches

            if (TryEnterCriticalSection(&m_AddCriticalSection) != FALSE)
            {
                // Now, no one can add to the queue

                if (m_GetIndex == m_AddIndex)
                {
                    // See if we can just shrink it... It is safe to use HeapReAlloc() because the queue is empty
                    auto return_value = ::HeapReAlloc(m_Heap, HEAP_NO_SERIALIZE, m_Items, WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS * sizeof(void *));

                    if (return_value != nullptr)
                    {
                        m_Items = static_cast<void **>(return_value);
                    }
                    else
                    {
                        // Looks like we'll have to do it the hard way
                        ::HeapFree(m_Heap, HEAP_NO_SERIALIZE, m_Items);
                        m_Items = (void **) ::HeapAlloc(m_Heap, HEAP_NO_SERIALIZE, WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS * sizeof(void *));
                    }

                    m_Size = (m_Items == nullptr) ? 0 : WFC_QUEUE_DEFAULT_NUMBER_OF_ITEMS;
                    m_AddIndex = 0;
                    m_GetIndex = 0;
                }
                else
                {
                    // m_GetIndex != m_AddIndex, this means that someone added
                    // to the queue between the time we checked m_Size for being
                    // too big and the time we entered the add critical section.
                    // If this happened then we are too busy to shrink
                }

                // Let people add to the queue once again

                ::LeaveCriticalSection(&m_AddCriticalSection);
            }
        }

        // Let other threads call Get() now, we are empty
        ::LeaveCriticalSection(&m_GetCriticalSection);
        return(false);
    }

    item = m_Items[m_GetIndex];

    // Make sure m_GetIndex is never invalid

    m_GetIndex = ((m_GetIndex + 1) >= m_Size) ? 0 : m_GetIndex + 1;

    // Let other threads call Get() now
    ::LeaveCriticalSection(&m_GetCriticalSection);

    return(true);
}

inline _Check_return_ std::size_t CQueue::GetLength(void) const noexcept
{
    // This is a very expensive process!
    // No one can call Add() or Get() while we're computing this

    std::size_t number_of_items_in_the_queue = 0;

    ::EnterCriticalSection(const_cast<CRITICAL_SECTION *>(&m_AddCriticalSection));
    ::EnterCriticalSection(const_cast<CRITICAL_SECTION *>(&m_GetCriticalSection));

    number_of_items_in_the_queue = (m_AddIndex >= m_GetIndex) ?
        (m_AddIndex - m_GetIndex) :
        ((m_AddIndex + m_Size) - m_GetIndex);

    ::LeaveCriticalSection(const_cast<CRITICAL_SECTION *>(&m_GetCriticalSection));
    ::LeaveCriticalSection(const_cast<CRITICAL_SECTION *>(&m_AddCriticalSection));

    return(number_of_items_in_the_queue);
}

inline void CQueue::m_GrowBy(_In_ std::size_t number_of_new_items) noexcept
{
    // We don't need to worry about critical sections because they
    // have both already been entered.
    // m_GrowBy() is only called from Add();

    if (number_of_new_items > CQUEUE_MAX_GROW_SIZE)
    {
        number_of_new_items = CQUEUE_MAX_GROW_SIZE;
    }

    std::size_t const new_size = m_Size + number_of_new_items;

    // 2000-05-16
    // Thanks go to Royce Mitchell III (royce3@aim-controls.com) for finding
    // a HUGE bug here. I was using HeapReAlloc as a short cut but my logic
    // was flawed. In certain circumstances, queue items were being dropped.

    void**  new_array = (void **) ::HeapAlloc(m_Heap, HEAP_NO_SERIALIZE | HEAP_CREATE_ALIGN_16, new_size * sizeof(void *));

    if (new_array != nullptr)
    {
        // Now copy all of the old items from the old queue to the new one.

        // Get the entries from the get-index to the end of the array
        ::CopyMemory(new_array, &m_Items[m_GetIndex], (m_Size - m_GetIndex) * sizeof(void *));

        // Get the entries from the beginning of the array to the add-index
        ::CopyMemory(&new_array[m_Size - m_GetIndex], m_Items, m_AddIndex * sizeof(void *));

        m_AddIndex = m_Size;
        m_GetIndex = 0;
        m_Size = new_size;
        void** pointer_to_free = m_Items;
        m_Items = new_array;

        ::HeapFree(m_Heap, HEAP_NO_SERIALIZE, pointer_to_free);
    }
}

#endif // QUEUE_CLASS_HEADER
