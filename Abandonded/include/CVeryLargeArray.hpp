#if ! defined( VERY_LARGE_ARRAY_CLASS_HEADER )

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

#define VERY_LARGE_ARRAY_CLASS_HEADER

#if defined( WFC_WORK_IN_PROGRESS )

#if ! defined( WFC_DEFAULT_VLA_PAGE_SIZE )
#define WFC_DEFAULT_VLA_PAGE_SIZE (65536)
#endif // WFC_DEFAULT_VAL_PAGE_SIZE

#define WFC_VLA_CONTINUE_PROCESSING (1)
#define WFC_VLA_ABORT_PROCESSING    (0)

typedef int (*WFC_VLA_ELEMENT_CALLBACK)( void * parameter, void * element_p );

class CVeryLargeArray
{
   private:

      // Don't allow canonical behavior (i.e. don't allow this class
      // to be passed by value)

      CVeryLargeArray( const CVeryLargeArray& ) {};
      CVeryLargeArray& operator=( const CVeryLargeArray& ) { return( *this ); };

   protected:

      unsigned __int64 m_NumberOfBytesPerElement;
      unsigned __int64 m_NumberOfBytesPerPage;
      unsigned __int64 m_NumberOfElementsInArray;
      unsigned __int64 m_NumberOfPagesInArray;

   public:

      CVeryLargeArray( unsigned __int64 number_of_bytes_per_element, unsigned __int64 number_of_bytes_per_page = WFC_DEFAULT_VLA_PAGE_SIZE );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CVeryLargeArray();

      // The easy stuff...

      virtual inline unsigned __int64 GetSize(          void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_NumberOfElementsInArray ); }
      virtual inline   signed __int64 GetUpperBound(    void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( GetSize() - 1             ); }
      virtual inline unsigned __int64 GetNumberOfPages( void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_NumberOfPagesInArray    ); }
      virtual inline unsigned __int64 GetPageSize(      void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_NumberOfBytesPerPage    ); }
      virtual inline unsigned __int64 GetElementSize(   void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_NumberOfBytesPerElement ); }

      // Default implementations
      virtual void Append( const void * element, unsigned __int64 number_of_elements_to_append = 1 ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void Append( CVeryLargeArray& source )                                                 WFC_DOESNT_THROW_EXCEPTIONS;

      // Overrides (pure virtual)

      virtual bool Add( const void * element )                                                                                     WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool Coalesce( void )                                                                                                WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool Copy( const CVeryLargeArray& source )                                                                           WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool ForEach( WFC_VLA_ELEMENT_CALLBACK callback, void * callback_context )                                           WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual void FreeExtra( void )                                                                                               WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool GetAt( unsigned __int64 array_index, void * destination ) const                                                 WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool InsertAt( unsigned __int64 array_index, void * new_element, unsigned __int64 number_of_times_to_insert_it = 1 ) WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool InsertAt( unsigned __int64 array_index, CVeryLargeArray& source )                                               WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual void RemoveAll( void )                                                                                               WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool RemoveAt( unsigned __int64 array_index, unsigned __int64 number_of_elements_to_remove = 1 )                     WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool SetSize( unsigned __int64 number_of_elements )                                                                  WFC_DOESNT_THROW_EXCEPTIONS = 0;
      virtual bool SetAt( unsigned __int64 array_index, void * new_element )                                                       WFC_DOESNT_THROW_EXCEPTIONS = 0;
};

#endif // WFC_WORK_IN_PROGRESS

#endif // VERY_LARGE_ARRAY_CLASS_HEADER
