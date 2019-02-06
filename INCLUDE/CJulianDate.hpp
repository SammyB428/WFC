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
** $Workfile: CJulianDate.hpp $
** $Revision: 6 $
** $Modtime: 6/26/01 11:02a $
*/

#if ! defined ( JULIAN_DATE_CLASS_HEADER )

#define JULIAN_DATE_CLASS_HEADER

class CJulianDate // Don't use this, I'm not finished with it yet
{
   protected:

      double m_JulianDays;

   public:

      CJulianDate() { m_JulianDays = 0.0; };
      CJulianDate( __in const CJulianDate& source ) { m_JulianDays = source.m_JulianDays; };

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CJulianDate();

      /*
      ** Methods
      */

      inline  void   Copy( __in const CJulianDate& source ) noexcept { m_JulianDays = source.m_JulianDays; }; // Don't use this
      inline  __checkReturn double Get( void ) const noexcept { return( m_JulianDays ); }; // Don't use this
      virtual __checkReturn int    GetDayOfWeek( void ) const noexcept; // Don't use this
      virtual __checkReturn bool   Set( __in const int year, __in const int month, __in const int day, __in const int hours = 0, __in const int minutes = 0, __in const int seconds = 0 ) noexcept; // Don't use this

      /*
      ** Operators
      */

      inline CJulianDate& operator=( __in const CJulianDate& source ) noexcept { m_JulianDays = source.m_JulianDays; return( *this ); };

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};   

#endif // JULIAN_DATE_CLASS_HEADER
