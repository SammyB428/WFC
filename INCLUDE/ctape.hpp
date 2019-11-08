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
** $Workfile: ctape.hpp $
** $Revision: 15 $
** $Modtime: 6/26/01 11:04a $
*/

#if ! defined( TAPE_CLASS_HEADER )

#define TAPE_CLASS_HEADER

class CTapeGetDriveParameters : public _TAPE_GET_DRIVE_PARAMETERS
{
   public:

      CTapeGetDriveParameters();
      CTapeGetDriveParameters( __in CTapeGetDriveParameters const& source );
      CTapeGetDriveParameters( __in TAPE_GET_DRIVE_PARAMETERS const * source );

      virtual ~CTapeGetDriveParameters();

      virtual void Copy( __in CTapeGetDriveParameters const& source ) noexcept;
      virtual void Copy( __in TAPE_GET_DRIVE_PARAMETERS const * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeGetDriveParameters& operator=( __in CTapeGetDriveParameters const&   source ) noexcept;
      virtual CTapeGetDriveParameters& operator=( __in TAPE_GET_DRIVE_PARAMETERS const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTapeGetMediaParameters : public _TAPE_GET_MEDIA_PARAMETERS
{
   public:

      CTapeGetMediaParameters();
      CTapeGetMediaParameters( __in CTapeGetMediaParameters const& source );
      CTapeGetMediaParameters( __in TAPE_GET_MEDIA_PARAMETERS const * source );

      virtual ~CTapeGetMediaParameters();

      virtual void Copy( __in CTapeGetMediaParameters const& source ) noexcept;
      virtual void Copy( __in TAPE_GET_MEDIA_PARAMETERS const * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeGetMediaParameters& operator = ( __in CTapeGetMediaParameters const&   source ) noexcept;
      virtual CTapeGetMediaParameters& operator = ( __in TAPE_GET_MEDIA_PARAMETERS const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTapeSetDriveParameters : public _TAPE_SET_DRIVE_PARAMETERS
{
   public:

      CTapeSetDriveParameters() noexcept;
      CTapeSetDriveParameters( __in CTapeSetDriveParameters const& source ) noexcept;
      CTapeSetDriveParameters( __in TAPE_SET_DRIVE_PARAMETERS const * source ) noexcept;

      virtual ~CTapeSetDriveParameters();

      virtual void Copy( __in CTapeSetDriveParameters const& source ) noexcept;
      virtual void Copy( __in TAPE_SET_DRIVE_PARAMETERS const * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeSetDriveParameters& operator=( __in CTapeSetDriveParameters const&   source ) noexcept;
      virtual CTapeSetDriveParameters& operator=( __in TAPE_SET_DRIVE_PARAMETERS const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTapeSetMediaParameters : public _TAPE_SET_MEDIA_PARAMETERS
{
   public:

      CTapeSetMediaParameters() noexcept;
      CTapeSetMediaParameters( __in CTapeSetMediaParameters const& source ) noexcept;
      CTapeSetMediaParameters( __in TAPE_SET_MEDIA_PARAMETERS const * source ) noexcept;

      virtual ~CTapeSetMediaParameters();

      virtual void Copy( __in CTapeSetMediaParameters const& source ) noexcept;
      virtual void Copy( __in TAPE_SET_MEDIA_PARAMETERS const * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeSetMediaParameters& operator=( __in CTapeSetMediaParameters const&   source ) noexcept;
      virtual CTapeSetMediaParameters& operator=( __in TAPE_SET_MEDIA_PARAMETERS const& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTape : public CDummyFile
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

       LPVOID m_BackupReadContextPointer{ nullptr };
       LPVOID m_BackupWriteContextPointer{ nullptr };

       bool m_AutomaticallyClose{ false };

       DWORD m_TapeDriveNumberStartingAtZero{ 0 };

   public:

       CTape(__in CTape const&) = delete;
       CTape& operator=(__in CTape const&) = delete;
 
       CTape() noexcept;

      virtual ~CTape();

      enum class TypeOfPartition : uint32_t
      {
         Fixed     = TAPE_FIXED_PARTITIONS,
         Initiator = TAPE_INITIATOR_PARTITIONS,
         Select    = TAPE_SELECT_PARTITIONS 
      };

      enum class TypeOfErasure : uint32_t
      {
         Long  = TAPE_ERASE_LONG,
         Short = TAPE_ERASE_SHORT
      };

      enum class TypeOfMark : uint32_t
      {
         File      = TAPE_FILEMARKS,
         LongFile  = TAPE_LONG_FILEMARKS,
         Set       = TAPE_SETMARKS,
         ShortFile = TAPE_SHORT_FILEMARKS
      };

      enum class Position : uint32_t
      {
         Absolute = TAPE_ABSOLUTE_POSITION,
         Logical  = TAPE_LOGICAL_POSITION
      };

      enum class PositionMethod : uint32_t
      {
         AbsoluteBlock       = TAPE_ABSOLUTE_BLOCK,
         LogicalBlock        = TAPE_LOGICAL_BLOCK,
         RewindTape          = TAPE_REWIND,
         EndOfData           = TAPE_SPACE_END_OF_DATA,
         FileMarks           = TAPE_SPACE_FILEMARKS,
         RelativeBlocks      = TAPE_SPACE_RELATIVE_BLOCKS,
         SequentialFileMarks = TAPE_SPACE_SEQUENTIAL_FMKS,
         SequentialSetMarks  = TAPE_SPACE_SEQUENTIAL_SMKS,
         SetMarks            = TAPE_SPACE_SETMARKS
      };

      enum class Operation : uint32_t
      {
         FormatTape  = TAPE_FORMAT,
         LoadTape    = TAPE_LOAD,
         LockTape    = TAPE_LOCK,
         TensionTape = TAPE_TENSION,
         UnloadTape  = TAPE_UNLOAD,
         UnlockTape  = TAPE_UNLOCK
      };

      /*
      ** The Win32 API
      */

      virtual _Check_return_ bool BackupRead( __out_bcount( number_of_bytes_to_read ) uint8_t * buffer,
                               __in DWORD const number_of_bytes_to_read,
                               __out LPDWORD     number_of_bytes_read,
                               __in bool        abort = false,
                               __in bool        restore_security_data = true ) noexcept; // BackupRead

      virtual _Check_return_ bool BackupSeek( __in DWORD const seek_low,
                               __in DWORD const seek_high,
                               __out LPDWORD seeked_low,
                               __out LPDWORD seeked_high ) noexcept; // BackupSeek

      virtual _Check_return_ bool BackupWrite( __in_bcount( number_of_bytes_to_write ) LPBYTE buffer,
                                __in DWORD const number_of_bytes_to_write,
                                __out LPDWORD     number_of_bytes_written,
                                __in bool        abort = false,
                                __in bool        restore_security_data = true ) noexcept; // BackupWrite

      virtual bool CreatePartition( __in TypeOfPartition const type_of_partition,
                                    __in DWORD const number_of_partitions,
                                    __in DWORD const number_of_megabytes_in_each_partition ) noexcept; // CreateTapePartition

      virtual bool Erase( __in TypeOfErasure const type_of_erasure, __in bool const return_immediately = false ) noexcept; // EraseTape

      virtual bool GetParameters( __out CTapeGetDriveParameters& parameters ) noexcept; // GetTapeParameters
      virtual bool GetParameters( __out CTapeGetMediaParameters& parameters ) noexcept;

      virtual bool GetPosition( __in Position const type_of_position_to_get,
                                __out DWORD& partition_number,
                                __out DWORD& position_low,
                                __out DWORD& position_high ) noexcept; // GetTapePosition

      virtual _Check_return_ DWORD GetStatus( void ) const noexcept; // GetTapeStatus
      virtual _Check_return_ DWORD GetDriveNumber( void ) const noexcept;

      virtual _Check_return_ bool Prepare( __in Operation const what_to_do, __in bool const return_immediately = false ) noexcept; // PrepareTape

      virtual _Check_return_ bool Rewind( void ) noexcept;

      inline constexpr _Check_return_ bool SetAutomaticallyClose(_In_ bool auto_close) noexcept { auto const return_value = m_AutomaticallyClose; m_AutomaticallyClose = auto_close; return(return_value); }; // returns previous setting
      virtual _Check_return_ bool SetParameters( __in CTapeSetDriveParameters const& parameters ) noexcept; // SetTapeParameters
      virtual _Check_return_ bool SetParameters( __in CTapeSetMediaParameters const& parameters ) noexcept;

      virtual _Check_return_ bool SetPosition( __in PositionMethod const how_to_get_there,
                                __in DWORD const         partition_number,
                                __in DWORD const         position_low,
                                __in DWORD const         position_high,
                                __in bool const          return_immediately = false ) noexcept; // SetTapePosition

      virtual _Check_return_ bool Mark( __in TypeOfMark const type_of_mark,
                         __in DWORD const     number_of_marks_to_write,
                         __in bool const      return_immediately = false ) noexcept; // WriteTapemark

      /*
      ** API's to make life easier
      */

      virtual _Check_return_ bool Open( __in UINT const tape_drive_number_starting_at_zero = 0 ) noexcept;
      virtual _Check_return_ bool Load( void ) noexcept;
      virtual _Check_return_ bool Lock( void ) noexcept;
      virtual _Check_return_ bool Unload( void ) noexcept;
      virtual _Check_return_ bool Unlock( void ) noexcept;
      virtual void Close( void ) noexcept override; // Calls Prepare( TAPE_UNLOAD )
};

#endif // TAPE_CLASS_HEADER
