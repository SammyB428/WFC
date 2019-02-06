/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
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
** $Workfile: CNetworkScheduleJob.hpp $
** $Revision: 7 $
** $Modtime: 6/26/01 11:03a $
*/

#if ! defined( NETWORK_SCHEDULER_CLASS_HEADER )

#define NETWORK_SCHEDULER_CLASS_HEADER

class CNetworkScheduleJob
{
   private:

      void m_Initialize( void ) noexcept;

   public:

      CNetworkScheduleJob();

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      CNetworkScheduleJob( __in const AT_ENUM * information_p );
      CNetworkScheduleJob( __in const AT_INFO * information_p );
      CNetworkScheduleJob( __in const CNetworkScheduleJob& source );
      virtual ~CNetworkScheduleJob();

      DWORD   ID;
      DWORD   NumberOfMillisecondsFromMidnight;
      DWORD   DaysOfTheMonth;
      DWORD   DaysOfTheWeek;
      DWORD   Flags;
      std::wstring Command;

      /*
      ** Can't make Copy take a const pointer because Microsoft screwed up the 
      ** net API header files...
      */

      virtual void Copy( __in const AT_ENUM * source ) noexcept;
      virtual void Copy( __in const AT_INFO * source ) noexcept;
      virtual void Copy( __in const CNetworkScheduleJob& source ) noexcept;
      virtual void Empty( void );
      virtual const CNetworkScheduleJob& operator = ( __in const CNetworkScheduleJob& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#if 0
class CNetworkScheduler : public CNetwork
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      /*
      ** Workstation information variables
      */

      FILE_INFO_3 * m_103InformationBuffer;

      /*
      ** File Information enumeration variables
      */

      DWORD m_103Index;
      DWORD m_103ResumeHandle;
      DWORD m_103CurrentEntryNumber;
      DWORD m_103NumberOfEntriesRead;
      DWORD m_103TotalNumberOfEntries;

      BOOL m_GetChunk( void ) noexcept;

   public:

       CNetworkFiles(const CNetworkFiles&) = delete;
       CNetworkFiles& operator=(const CNetworkFiles&) = delete;
       CNetworkFiles();
      CNetworkFiles( LPCTSTR machine_name );
      virtual ~CNetworkFiles();

      virtual void  Close( void ) noexcept;
      virtual BOOL  Close( CNetworkFileInformation& file_to_close ) noexcept;
      virtual BOOL  Enumerate( void ) noexcept;
      virtual BOOL  GetNext( CNetworkFileInformation& information ) noexcept;
};

#endif

#endif // NETWORK_SCHEDULER_CLASS_HEADER
