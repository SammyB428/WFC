/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2014, Samuel R. Blackburn
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
** $Workfile: wfc_convert_double_null_terminated_string.cpp $
** $Revision: 7 $
** $Modtime: 6/26/01 10:57a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

void PASCAL Win32FoundationClasses::wfc_convert_double_null_terminated_string(__in const BYTE * buffer, __inout std::vector<std::wstring>& string_array)
{
    // Always start with a virgin array
    string_array.clear();

    if (buffer == nullptr)
    {
        return;
    }

    // We were passed a pointer, don't trust it

    std::wstring string_to_add;

    WFC_TRY
    {
        while (buffer[0] != 0x00)
        {
            copy( string_to_add, (LPCTSTR)buffer);

            buffer += (_tcslen((LPCTSTR)buffer) * sizeof(TCHAR)) + 1;
        }
    }
        WFC_CATCH_ALL
    {
        return;
    }
        WFC_END_CATCH_ALL
}

void PASCAL Win32FoundationClasses::wfc_convert_double_null_terminated_string( __in const BYTE * buffer, __inout CStringArray& string_array )
{
   // Always start with a virgin array
   string_array.RemoveAll();

   if ( buffer == nullptr )
   {
      return;
   }

   // We were passed a pointer, don't trust it

   WFC_TRY
   {
      while ( buffer[ 0 ] != 0x00 )
      {
         (void) string_array.Add( (LPCTSTR) buffer );

         buffer += ( _tcslen( (LPCTSTR) buffer ) * sizeof( TCHAR ) ) + 1;
      }
   }
   WFC_CATCH_ALL
   {
      return;
   }
   WFC_END_CATCH_ALL
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - wfc_convert_double_null_terminated_string</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32">
<META name="description" content="Simple C function that converts a double-NULL terminated string to a CStringArray object (which may be ASCII).">
</HEAD>

<BODY>

<H1>wfc_convert_double_null_terminated_string</H1>
$Revision: 7 $<HR>

<H2>Declaration</H2>
<PRE><CODE>void wfc_convert_double_null_terminated_string( const BYTE * unicode_string, buffer, CStringArray&amp; string_array ) )</CODE></PRE>

<H2>Description</H2>
This function converts a double-NULL terminated string to a CStringArray object.

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   TCHAR dos_device_names[ 4096 ];

   ZeroMemory( dos_device_names, sizeof( dos_device_names ) );

   DWORD return_value = 0;

   return_value = QueryDosDevice( nullptr, dos_device_names, std::size( dos_device_names ) );

   CStringArray names;

   <B>wfc_convert_double_null_terminated_string</B>( reinterpret_cast&lt; const BYTE * &gt;( dos_device_names ), names );

   int loop_index         = 0;
   int number_of_elements = names.GetSize();

   while( loop_index < number_of_elements )
   {
      _tprintf( TEXT( &quot;%s\n&quot; ), (LPCTSTR) names.GetAt( loop_index ) );
      WFCTRACEVAL( TEXT( &quot;Name: &quot; ), names.GetAt( loop_index ) );
      loop_index++;
   }

   return( EXIT_SUCCESS );
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: wfc_convert_double_null_terminated_string.cpp $<BR>
$Modtime: 6/26/01 10:57a $
</BODY>

</HTML>
*/
