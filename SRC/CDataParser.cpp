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
** $Workfile: CDataParser.cpp $
** $Revision: 34 $
** $Modtime: 8/29/01 7:42p $
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

void CDataParser::GetTextStatistics( _In_ CParsePoint& parse_point, _Inout_ CTextStatistics& statistics ) const noexcept
{
   statistics.Empty();

   CParsePoint here( parse_point );

   uint32_t last_character = 0;
   uint32_t this_character = 0;

   while( GetNextCharacter( here, this_character ) == true )
   {
      statistics.NumberOfCharacters++;

      switch( this_character )
      {
         case '(':

            statistics.Parentheses.Open++;
            statistics.NumberOfPunctuation++;
            break;

         case ')':

            statistics.Parentheses.Close++;
            statistics.NumberOfPunctuation++;

            if ( last_character == '(' )
            {
               statistics.Parentheses.BackToBack++;
            }

            break;

         case '[':

            statistics.Brackets.Open++;
            statistics.NumberOfPunctuation++;
            break;

         case ']':

            statistics.Brackets.Close++;
            statistics.NumberOfPunctuation++;

            if ( last_character == '[' )
            {
               statistics.Brackets.BackToBack++;
            }

            break;

         case '<':

            statistics.GreaterThanLessThan.Open++;
            statistics.NumberOfPunctuation++;
            break;

         case '>':

            statistics.GreaterThanLessThan.Close++;
            statistics.NumberOfPunctuation++;

            if ( last_character == '<' )
            {
               statistics.GreaterThanLessThan.BackToBack++;
            }
            else if ( last_character == '-' )
            {
               statistics.NumberOfArrows++;
            }

            break;

         case '{':

            statistics.CurlyBraces.Open++;
            statistics.NumberOfPunctuation++;
            break;

         case '}':

            statistics.CurlyBraces.Close++;
            statistics.NumberOfPunctuation++;

            if ( last_character == '{' )
            {
               statistics.CurlyBraces.BackToBack++;
            }

            break;

         case ' ':
         case TAB_CHARACTER:

            statistics.NumberOfSpaces++;
            break;

         case LINE_FEED:

            statistics.NumberOfLines++;
            break;

         case CARRIAGE_RETURN:

            if ( last_character != LINE_FEED )
            {
               statistics.NumberOfLines++;
            }

            break;

         case 34: // double quote

            statistics.NumberOfDoubleQuotes++;
            statistics.NumberOfPunctuation++;
            break;

         case 39: // single quote

            statistics.NumberOfSingleQuotes++;
            statistics.NumberOfPunctuation++;
            break;

         case '=':

            statistics.NumberOfEquals++;
            statistics.NumberOfPunctuation++;
            break;

         case ':':

            statistics.NumberOfColons++;
            statistics.NumberOfPunctuation++;
            break;

         case ';':

            statistics.NumberOfSemiColons++;
            statistics.NumberOfPunctuation++;
            break;

         default:

            if ( this_character >= '0' && this_character <= '9' )
            {
               statistics.NumberOfDigits++;
            }
            else if ( this_character >= 'a' && this_character <= 'z' )
            {
               statistics.NumberOfLetters++;
            }
            else if ( this_character >= 'A' && this_character <= 'Z' )
            {
               statistics.NumberOfLetters++;
            }
            else if ( wfc_is_punctuation( this_character ) == true )
            {
               statistics.NumberOfPunctuation++;
            }

            break;
      }

      last_character = this_character;
   }
}


#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CDataParser::Dump( CDumpContext& dump_context ) const
{
   dump_context << TEXT( " is a CDataParser at " ) << (VOID *) this << TEXT( "\n{\n" );
   dump_context << TEXT( "   m_AutomaticallyDelete is " );

   if ( m_AutomaticallyDelete == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "   m_IsASCII is " );

   if ( m_IsASCII == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "   m_IsUCS4 is " );

   if ( m_IsUCS4 == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "   m_IsBigEndian is " );

   if ( m_IsBigEndian == false )
   {
      dump_context << TEXT( "false\n" );
   }
   else
   {
      dump_context << TEXT( "true\n" );
   }

   dump_context << TEXT( "   m_Data is " );

   if ( m_Data == nullptr )
   {
      dump_context << TEXT( "NULL\n" );
   }
   else
   {
      m_Data->Dump( dump_context );
   }

   dump_context << TEXT( "}\n" );

}

#endif // _DEBUG

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CDataParser</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, push technology, source code">
<META name="description" content="The C++ class that parses anything.">
</HEAD>

<BODY>

<H1>CDataParser</H1>

$Revision: 34 $
<HR>

<H2>Description</H2>

This class is a generic class to assist in parsing data.
It provides some basic searching capability as well as
idiot proofed retrieval.

<H2>Construction</H2>

<DL COMPACT>

<DT><PRE><B>CDataParser</B>()</PRE><DD>
Constructs the object.
</DL>

<H2>Methods</H2>

<DL COMPACT>

<DT><PRE>void <B><A NAME="AdvanceByOneCharacter">AdvanceByOneCharacter</A></B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, DWORD character = 0 ) const</PRE><DD>
Advances the parse point by at least one character. <CODE>parse_point</CODE>
will be incremented (see <A HREF="CParsePoint.htm#AutoIncrement">AutoIncrement()</A>)
by the character it finds. If you want to be incremented based on
a character that you want, put that character's value in <CODE>character</CODE>.

<DT><PRE>void <B><A NAME="Empty">Empty</A></B>( void )</PRE><DD>
Re-initializes the object. If a std::vector&lt;uint8_t&gt; was attached
(and set to automatically delete) it will be deleted.

<DT><PRE>BOOL <B><A NAME="Find">Find</A></B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, BYTE byte_to_find, <A HREF="CParsePoint.htm">CParsePoint</A>&amp; found_at ) const
BOOL <B>Find</B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, const std::wstring&amp; string_to_find, <A HREF="CParsePoint.htm">CParsePoint</A>&amp; found_at ) const
BOOL <B>Find</B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, const std::vector&lt;uint8_t&gt;&amp; bytes_to_find, <A HREF="CParsePoint.htm">CParsePoint</A>&amp; found_at ) const</PRE><DD>
Searches for <CODE>byte_to_find</CODE>, <CODE>string_to_find</CODE> or
<CODE>bytes_to_find</CODE> beginning at <CODE>parse_point</CODE>.
If what you&#39;re looking for is found, the location will be put into
<CODE>found_at</CODE> and the return value will be TRUE. If <B>Find</B>()
cannot find what you're looking for, it will return FALSE.

<DT><PRE>BOOL <B><A NAME="FindNoCase">FindNoCase</A></B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, const std::wstring&amp; string_to_find, <A HREF="CParsePoint.htm">CParsePoint</A>&amp; found_at ) const
BOOL <B>FindNoCase</B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, const std::vector&lt;uint8_t&gt;&amp; bytes_to_find, <A HREF="CParsePoint.htm">CParsePoint</A>&amp; found_at ) const</PRE><DD>
Will search for <CODE>string_to_find</CODE> or <CODE>bytes_to_find</CODE> without
regard to case. It will match 'a' with 'A'.
If what you're looking for is found, the location will be put into
<CODE>found_at</CODE> and the return value will be TRUE. If <B>FindNoCase</B>()
cannot find what you're looking for, it will return FALSE.

<DT><PRE>BOOL <B><A NAME="Get">Get</A></B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, DWORD length, std::vector&lt;uint8_t&gt;&amp; bytes_to_get ) const
BOOL <B>Get</B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, DWORD length, std::wstring&amp; string_to_get ) const</PRE><DD>
Retrieves the <CODE>length</CODE> number of bytes beginning at <CODE>parse_point</CODE>.

<DT><PRE>BYTE <B><A NAME="GetAt">GetAt</A></B>( DWORD index ) const</PRE><DD>
Retrieves the byte at the given <CODE>index</CODE>.

<DT><PRE>DWORD <B><A NAME="GetCharacter">GetCharacter</A></B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; const_parse_point, DWORD const number_of_characters_ahead = 0 ) const</PRE><DD>
Returns a character at the given location. <B>NOTE:<B> Don't assume that characters
are one byte each like in ASCII. Characters can be made up of multiple bytes
each. This will happen when <A HREF="#SetTextToASCII">SetTextToASCII()</A> is set to
FALSE or <A HREF="#SetTextToUCS4">SetTextToUCS4()</A> is set to TRUE.

<DT><PRE>void <B><A NAME="GetLine">GetLine</A></B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, std::wstring&amp; line )</PRE><DD>
This method will retrieve a line of text from the data.
A line is terminated by a carriage return, line feed or both.
The returned line will <B><I>not</I></B> contain the terminating carriage return/line feed character(s).

<DT><PRE>BOOL <B><A NAME="GetNextCharacter">GetNextCharacter</A></B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, DWORD&amp; character ) const</PRE><DD>
Like <A HREF="#GetCharacter">GetCharacter()</A> except the parse point will be advanced
by however many bytes make up one character (1, 2 or 4). It allows you to basically
enumerate through the data stream. It will return TRUE of <CODE>character</CODE> was
filled or FALSE if you have reached the end (or passed the end) of the data.

<DT><PRE>DWORD <B><A NAME="GetUCS4Order">GetUCS4Order</A></B>( void ) const</PRE><DD>
Returns one of the following:

<UL>
<LI>1234
<LI>2143
<LI>3412
<LI>4321
</UL>

<DT><PRE>BYTE <B><A NAME="GetUnicodeToASCIITranslationFailureCharacter">GetUnicodeToASCIITranslationFailureCharacter</A></B>( void ) const</PRE><DD>
Returns the ASCII character that will be substituted when a translation from
UNICODE to ASCII fails.

<DT><PRE>DWORD <B><A NAME="GetSize">GetSize</A></B>( void ) const</PRE><DD>
Returns the number of bytes in the data area.

<DT><PRE>BOOL <B><A NAME="GetUntilAndIncluding">GetUntilAndIncluding</A></B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, BYTE termination_byte, std::wstring&amp; string_to_get ) const
BOOL <B>GetUntilAndIncluding</B>( <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, BYTE termination_byte, std::vector&lt;uint8_t&gt;&amp; bytes_to_get ) const</PRE><DD>
This method retrieves data (filling <CODE>string_to_get</CODE> or <CODE>bytes_to_get</CODE>)
until and including the <CODE>termination_byte</CODE>. The <CODE>parse_point</CODE>
is advanced in the process.

<DT><PRE>BOOL <B><A NAME="Initialize">Initialize</A></B>( std::vector&lt;uint8_t&gt; * data, BOOL automatically_delete = FALSE )
BOOL <B>Initialize</B>( const std::vector&lt;std::wstring&gt;&amp; strings )</PRE><DD>
Tells the parser where to go for data.

<DT><PRE>BOOL <B><A NAME="IsTextASCII">IsTextASCII</A></B>( void ) const</PRE><DD>
Returns TRUE if characters are to be treated as one byte each.

<DT><PRE>BOOL <B><A NAME="IsTextBigEndian">IsTextBigEndian</A></B>( void ) const</PRE><DD>
Returns TRUE if text is big endian (Sun) format. This has meaning when the
underlying characters are treated as UNICODE or ICS-4.

<DT><PRE>BOOL <B><A NAME="IsTextUCS4">IsTextUCS4</A></B>( void ) const</PRE><DD>
Returns TRUE if characters are to be treated as four bytes per character.

<DT><PRE>BOOL <B><A NAME="PeekAtCharacter">PeekAtCharacter</A></B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, DWORD&amp; character, DWORD const number_of_characters_ahead = 1 ) const</PRE><DD>
Allows you to peek ahead at characters. It will return TRUE if
<CODE>character</CODE> was filled with a character from the data stream.
It will return FALSE when you have tried to read passed the end of the stream.

<DT><PRE>DWORD <B><A NAME="PeekCharacter">PeekCharacter</A></B>( const <A HREF="CParsePoint.htm">CParsePoint</A>&amp; parse_point, const LONG number_of_characters_ahead ) const</PRE><DD>
Allows you to peek ahead at characters. It will the character at the current location plus
<CODE>number_of_characters_ahead</CODE>. If you attempt to read a character passed the
end of the data, it will return NULL.

<DT><PRE>BOOL <B><A NAME="SetTextToASCII">SetTextToASCII</A></B>( BOOL text_is_ascii = TRUE )</PRE><DD>
Tells the class to interpret characters as one byte each.

<DT><PRE>BOOL <B><A NAME="SetTextToBigEndian">SetTextToBigEndian</A></B>( BOOL unicode_is_big_endian = TRUE )</PRE><DD>
Tells the class to interpret UNICODE or UCS-4 characters as big endian (Sun) format.
Little endian is Intel format.

<DT><PRE>BOOL <B><A NAME="SetTextToUCS4">SetTextToUCS4</A></B>( BOOL text_is_ucs4 = TRUE )</PRE><DD>
Tells the class to interpret characters as four bytes each.

<DT><PRE>BOOL <B><A NAME="SetUCS4Order">SetUCS4Order</A></B>( DWORD order = 4321 )</PRE><DD>
Tells the parser to interpret UCS-4 characters in 4321 format.

<DT><PRE>void <B><A NAME="SetUnicodeToASCIITranslationFailureCharacter">SetUnicodeToASCIITranslationFailureCharacter</A></B>( BYTE asci_character )</PRE><DD>
This sets the character that will be substituted when a translation must be made
from UNICODE to ASCII. Since ASCII only has 256 possible values and UNICODE has 65536,
some provision must be made for bad translations.

</DL>

<H2>Example</H2>
<PRE><CODE>&#35;include &lt;wfc.h&gt;

BOOL parse_document( const std::wstring&amp; filename, <A HREF="CExtensibleMarkupLanguageDocument.htm">CExtensibleMarkupLanguageDocument</A>&amp; document )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;parse_document()&quot; ) );

   std::vector&lt;uint8_t&gt; bytes;

   if ( get_bytes( filename, bytes ) != TRUE )
   {
      return( FALSE );
   }

   <B>CDataParser</B> parser;

   parser.<A HREF="#Initialize">Initialize</A>( &amp;bytes, FALSE );

   if ( document.Parse( parser ) != FALSE )
   {
      WFCTRACE( TEXT( &quot;Parsed OK&quot; ) );
      return( TRUE );
   }
   else
   {
      WFCTRACE( TEXT( &quot;Can't parse document&quot; ) );
      return( FALSE );
   }
}</CODE></PRE>

<HR>
<I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CDataParser.cpp $<BR>
$Modtime: 8/29/01 7:42p $
</BODY>

</HTML>
The following line should go in AUTOEXP.DAT so the debugging tooltips will format properly
ToolTipFormatLine=CDataParser =size=<m_Data->m_nSize>
*/
