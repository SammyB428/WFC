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
** $Workfile: CWeather.hpp $
** $Revision: 8 $
** $Modtime: 6/26/01 11:05a $
*/

#if ! defined( WEATHER_CLASS_HEADER_FILE )

#define WEATHER_CLASS_HEADER_FILE

class CWeather
#if ! defined(  WFC_NO_SERIALIZATION )
: public CObject
#endif // WFC_NO_SERIALIZATION
{
#if ! defined(  WFC_NO_SERIALIZATION )
   DECLARE_SERIAL( CWeather )
#endif // WFC_NO_SERIALIZATION

   public:

      CWeather();
      CWeather( const CWeather& source );

      /*
      ** Destructor should be virtual according to MSJ article in Sept 1992
      ** "Do More with Less Code:..."
      */

      virtual ~CWeather();

      enum _TemperatureUnits
      {
         Farenheit,
         Celcius
      };

      enum _WindSpeedUnits
      {
         MilesPerHour,
         KilometersPerHour
      };

      enum _BarometricPressureUnits
      {
         Inches
      };

      /*
      ** Data
      */

      CString Location;
      CTime   Time;
      double  Temperature;
      int     TemperatureUnits;
      double  HumidityPercent;
      double  WindSpeed;
      int     WindSpeedUnits;
      CString WindDirection;
      double  BarometricPressure;
      int     BarometricPressureUnits;
      CString Description;

      /*
      ** Methods
      */

      virtual int  Compare( const CWeather& source ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void Copy( const CWeather& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual void Empty( void ) WFC_DOESNT_THROW_EXCEPTIONS;
#if ! defined(  WFC_NO_SERIALIZATION )
      virtual void Serialize( CArchive& archive );
#endif // WFC_NO_SERIALIZATION

      /*
      ** Operators
      */

      virtual CWeather& operator = ( const CWeather& source ) WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL operator == ( const CWeather& source ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL operator <  ( const CWeather& source ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL operator >  ( const CWeather& source ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL operator <= ( const CWeather& source ) const WFC_DOESNT_THROW_EXCEPTIONS;
      virtual BOOL operator >= ( const CWeather& source ) const WFC_DOESNT_THROW_EXCEPTIONS;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )
      virtual void Dump( CDumpContext& dump_context ) const;
#endif // _DEBUG
};

#endif // WEATHER_CLASS_HEADER_FILE
