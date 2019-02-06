/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2016, Samuel R. Blackburn
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
      CTapeGetDriveParameters( __in const CTapeGetDriveParameters& source );
      CTapeGetDriveParameters( __in const TAPE_GET_DRIVE_PARAMETERS * source );

      virtual ~CTapeGetDriveParameters();

      virtual void Copy( __in const CTapeGetDriveParameters& source ) noexcept;
      virtual void Copy( __in const TAPE_GET_DRIVE_PARAMETERS * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeGetDriveParameters& operator=( __in const CTapeGetDriveParameters&   source ) noexcept;
      virtual CTapeGetDriveParameters& operator=( __in const TAPE_GET_DRIVE_PARAMETERS& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTapeGetMediaParameters : public _TAPE_GET_MEDIA_PARAMETERS
{
   public:

      CTapeGetMediaParameters();
      CTapeGetMediaParameters( __in const CTapeGetMediaParameters& source );
      CTapeGetMediaParameters( __in const TAPE_GET_MEDIA_PARAMETERS * source );

      virtual ~CTapeGetMediaParameters();

      virtual void Copy( __in const CTapeGetMediaParameters& source ) noexcept;
      virtual void Copy( __in const TAPE_GET_MEDIA_PARAMETERS * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeGetMediaParameters& operator = ( __in const CTapeGetMediaParameters&   source ) noexcept;
      virtual CTapeGetMediaParameters& operator = ( __in const TAPE_GET_MEDIA_PARAMETERS& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTapeSetDriveParameters : public _TAPE_SET_DRIVE_PARAMETERS
{
   public:

      CTapeSetDriveParameters();
      CTapeSetDriveParameters( __in const CTapeSetDriveParameters& source );
      CTapeSetDriveParameters( __in const TAPE_SET_DRIVE_PARAMETERS * source );

      virtual ~CTapeSetDriveParameters();

      virtual void Copy( __in const CTapeSetDriveParameters& source ) noexcept;
      virtual void Copy( __in const TAPE_SET_DRIVE_PARAMETERS * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeSetDriveParameters& operator=( __in const CTapeSetDriveParameters&   source ) noexcept;
      virtual CTapeSetDriveParameters& operator=( __in const TAPE_SET_DRIVE_PARAMETERS& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTapeSetMediaParameters : public _TAPE_SET_MEDIA_PARAMETERS
{
   public:

      CTapeSetMediaParameters();
      CTapeSetMediaParameters( __in const CTapeSetMediaParameters& source );
      CTapeSetMediaParameters( __in const TAPE_SET_MEDIA_PARAMETERS * source );

      virtual ~CTapeSetMediaParameters();

      virtual void Copy( __in const CTapeSetMediaParameters& source ) noexcept;
      virtual void Copy( __in const TAPE_SET_MEDIA_PARAMETERS * source ) noexcept;
      virtual void Empty( void ) noexcept;

      virtual CTapeSetMediaParameters& operator=( __in const CTapeSetMediaParameters&   source ) noexcept;
      virtual CTapeSetMediaParameters& operator=( __in const TAPE_SET_MEDIA_PARAMETERS& source ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif
};

class CTape : public CDummyFile
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      LPVOID m_BackupReadContextPointer;
      LPVOID m_BackupWriteContextPointer;

      BOOL m_AutomaticallyClose;

      DWORD m_TapeDriveNumberStartingAtZero;

   public:

       CTape(__in const CTape&) = delete;
       CTape& operator=(__in const CTape&) = delete;
 
       CTape();

      virtual ~CTape();

      enum TypeOfPartition
      {
         Fixed     = TAPE_FIXED_PARTITIONS,
         Initiator = TAPE_INITIATOR_PARTITIONS,
         Select    = TAPE_SELECT_PARTITIONS 
      };

      enum TypeOfErasure
      {
         Long  = TAPE_ERASE_LONG,
         Short = TAPE_ERASE_SHORT
      };

      enum TypeOfMark
      {
         File      = TAPE_FILEMARKS,
         LongFile  = TAPE_LONG_FILEMARKS,
         Set       = TAPE_SETMARKS,
         ShortFile = TAPE_SHORT_FILEMARKS
      };

      enum Position
      {
         Absolute = TAPE_ABSOLUTE_POSITION,
         Logical  = TAPE_LOGICAL_POSITION
      };

      enum PositionMethod
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

      enum Operation
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

      virtual __checkReturn BOOL BackupRead( __out_bcount( number_of_bytes_to_read ) LPBYTE      buffer,
                               __in const DWORD number_of_bytes_to_read,
                               __out LPDWORD     number_of_bytes_read,
                               __in BOOL        abort = FALSE,
                               __in BOOL        restore_security_data = TRUE ) noexcept; // BackupRead

      virtual __checkReturn BOOL BackupSeek( __in const DWORD seek_low,
                               __in const DWORD seek_high,
                               __out LPDWORD seeked_low,
                               __out LPDWORD seeked_high ) noexcept; // BackupSeek

      virtual __checkReturn BOOL BackupWrite( __in_bcount( number_of_bytes_to_write ) LPBYTE buffer,
                                __in const DWORD number_of_bytes_to_write,
                                __out LPDWORD     number_of_bytes_written,
                                __in BOOL        abort = FALSE,
                                __in BOOL        restore_security_data = TRUE ) noexcept; // BackupWrite

      virtual BOOL CreatePartition( __in const TypeOfPartition type_of_partition,
                                    __in const DWORD number_of_partitions,
                                    __in const DWORD number_of_megabytes_in_each_partition ) noexcept; // CreateTapePartition

      virtual BOOL Erase( __in const TypeOfErasure type_of_erasure, __in const BOOL return_immediately = FALSE ) noexcept; // EraseTape

      virtual BOOL GetParameters( __out CTapeGetDriveParameters& parameters ) noexcept; // GetTapeParameters
      virtual BOOL GetParameters( __out CTapeGetMediaParameters& parameters ) noexcept;

      virtual BOOL GetPosition( __in const Position type_of_position_to_get,
                                __out DWORD& partition_number,
                                __out DWORD& position_low,
                                __out DWORD& position_high ) noexcept; // GetTapePosition

      virtual __checkReturn DWORD GetStatus( void ) const noexcept; // GetTapeStatus
      virtual __checkReturn DWORD GetDriveNumber( void ) const noexcept;

      virtual __checkReturn BOOL Prepare( __in const Operation what_to_do, __in const BOOL return_immediately = FALSE ) noexcept; // PrepareTape

      virtual __checkReturn BOOL Rewind( void ) noexcept;

      virtual __checkReturn BOOL SetAutomaticallyClose( __in BOOL auto_close ) noexcept; // returns previous setting
      virtual __checkReturn BOOL SetParameters( __in const CTapeSetDriveParameters& parameters ) noexcept; // SetTapeParameters
      virtual __checkReturn BOOL SetParameters( __in const CTapeSetMediaParameters& parameters ) noexcept;

      virtual __checkReturn BOOL SetPosition( __in const PositionMethod how_to_get_there,
                                __in const DWORD          partition_number,
                                __in const DWORD          position_low,
                                __in const DWORD          position_high,
                                __in const BOOL           return_immediately = FALSE ) noexcept; // SetTapePosition

      virtual __checkReturn BOOL Mark( __in const TypeOfMark type_of_mark,
                         __in const DWORD      number_of_marks_to_write, 
                         __in const BOOL       return_immediately = FALSE ) noexcept; // WriteTapemark

      /*
      ** API's to make life easier
      */

      virtual __checkReturn BOOL Open( __in const UINT tape_drive_number_starting_at_zero = 0 ) noexcept;
      virtual __checkReturn BOOL Load( void ) noexcept;
      virtual __checkReturn BOOL Lock( void ) noexcept;
      virtual __checkReturn BOOL Unload( void ) noexcept;
      virtual __checkReturn BOOL Unlock( void ) noexcept;
      virtual void Close( void ) noexcept; // Calls Prepare( TAPE_UNLOAD )
};

#endif // TAPE_CLASS_HEADER
