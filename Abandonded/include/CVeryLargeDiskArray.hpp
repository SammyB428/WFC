#if ! defined( VERY_LARGE_DISK_ARRAY_CLASS_HEADER )

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2003, Samuel R. Blackburn
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
** $Workfile: CCOMSucks.hpp $
** $Revision: 2 $
** $Modtime: 6/26/01 11:00a $
*/

#define VERY_LARGE_DISK_ARRAY_CLASS_HEADER

#if defined( WFC_WORK_IN_PROGRESS )

#if ! defined( VERY_LARGE_DISK_ARRAY_NODE_ID )
#define VERY_LARGE_DISK_ARRAY_NODE_ID (0x6D6153617275614C)
#endif // VERY_LARGE_DISK_ARRAY_NODE_ID

typedef struct _very_large_disk_array_node
{
   unsigned __int64 interface_id;
   unsigned __int64 number_of_elements_on_this_page;
   unsigned __int64 next_node;
   unsigned __int64 previous_node;
}
VERY_LARGE_DISK_ARRAY_NODE, *VERY_LARGE_DISK_ARRAY_NODE_P;

#define VERY_LARGE_DISK_ARRAY_NODE_OVERHEAD_SIZE ( sizeof( unsigned __int64 ) * 4 ) )

class CVeryLargeDiskArray : public CVeryLargeArray
{
   private:

      // Don't allow canonical behavior (i.e. don't allow this class
      // to be passed by value)

      CVeryLargeDiskArray& operator=( const CVeryLargeDiskArray& ) { return( *this ); };

   protected:

      HANDLE m_FileHandle;

   public:

      CVeryLargeDiskArray( unsigned __int64 number_of_bytes_per_element, unsigned __int64 number_of_bytes_per_page = WFC_DEFAULT_VLA_PAGE_SIZE );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CVeryLargeDiskArray();

      // Methods we have to override (in CVeryLargeArray)
      virtual bool Add( const void * element ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool Coalesce( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool Copy( const CVeryLargeArray& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool ForEach( WFC_VLA_ELEMENT_CALLBACK callback, void * callback_context ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void FreeExtra( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool GetAt( unsigned __int64 array_index, void * destination ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool InsertAt( unsigned __int64 array_index, void * new_element, unsigned __int64 number_of_times_to_insert_it = 1 ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool InsertAt( unsigned __int64 array_index, CVeryLargeArray& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void RemoveAll( void ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool RemoveAt( unsigned __int64 array_index, unsigned __int64 number_of_elements_to_remove = 1 ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool SetSize( unsigned __int64 number_of_elements ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual bool SetAt( unsigned __int64 array_index, void * new_element ) WFC_DOESNT_THROW_EXCEPTIONS;
};

#endif // WFC_WORK_IN_PROGRESS

#endif // VERY_LARGE_DISK_ARRAY_CLASS_HEADER
