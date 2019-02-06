#include <stlxml.h>
#pragma hdrstop

#if defined( _DEBUG )
#pragma comment( lib, "..\\..\\Debug\\stlxml.lib" )
#else
#pragma comment( lib, "..\\..\\Release\\stlxml.lib" )
#endif

void main( void )
{
   CExtensibleMarkupLanguageDocument xml;

   DWORD parse_options = 0;

   parse_options = xml.GetParseOptions();

   return;
}
