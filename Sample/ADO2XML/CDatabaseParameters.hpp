#if ! defined( DATABASE_PARAMETERS_HEADER_FILE )

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2003, Samuel R. Blackburn
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
** $Workfile: CDatabaseParameters.hpp $
** $Revision: 2 $
** $Modtime: 3/21/00 4:21a $
** $Reuse Tracing Code: 1 $
*/

#define DATABASE_PARAMETERS_HEADER_FILE

class CDatabaseParameters
{
   protected:

      CString m_Provider;
      CString m_UserID;
      CString m_Password;
      CString m_DataSource;
      CString m_InitialCatalog;
      CString m_ConnectionString;

   public:

      CDatabaseParameters();
      CDatabaseParameters( const CDatabaseParameters& source );
     ~CDatabaseParameters();

      // General bee-a-good-object methods

      void Empty( void );
      void Copy( const CDatabaseParameters& source );
      CDatabaseParameters& operator=( const CDatabaseParameters& source );

      // Data IO methods

      void GetProvider( CString& return_value ) const;
      void GetUserID( CString& return_value ) const;
      void GetPassword( CString& return_value ) const;
      void GetDataSource( CString& return_value ) const;
      void GetInitialCatalog( CString& return_value ) const;

      void SetProvider( LPCTSTR set_string );
      void SetUserID( LPCTSTR set_string );
      void SetPassword( LPCTSTR set_string );
      void SetDataSource( LPCTSTR set_string );
      void SetInitialCatalog( LPCTSTR set_string );
      void SetConnectionString( LPCTSTR set_string );

      // The reason for the class...

      void GetConnectionString( CString& return_value ) const;
};

#endif // DATABASE_PARAMETERS_HEADER_FILE
