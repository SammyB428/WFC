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
** $Workfile: CMicrosoftTape.hpp $
** $Revision: 11 $
** $Modtime: 6/26/01 11:02a $
*/

#if ! defined( MICROSOFT_TAPE_CLASS_HEADER )

#define MICROSOFT_TAPE_CLASS_HEADER

class CMicrosoftTape : public CTape
{
   private:

      void m_Initialize( void ) noexcept;

   protected:

      CMTFTape m_TapeHeader;

   public:

       CMicrosoftTape(const CMicrosoftTape&) = delete;
       CMicrosoftTape& operator=(const CMicrosoftTape&) = delete;
       CMicrosoftTape();

      virtual ~CMicrosoftTape();

      virtual void Empty( void ) noexcept;
      virtual __checkReturn BOOL EnumerateSets( void ) noexcept;
      virtual __checkReturn BOOL Open( __in const UINT tape_drive_number_starting_at_zero ) noexcept;
      virtual __checkReturn BOOL ReadHeader( void ) noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      void Dump( CDumpContext& dump_context ) const;

#endif
};

#endif // MICROSOFT_TAPE_CLASS_HEADER
