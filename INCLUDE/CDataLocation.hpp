#if ! defined ( DATA_PARSER_CLASS_HEADER )

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** You can use it any way you like as long as you don't try to sell it.
**
** Any attempt to sell WFC in source code form must have the permission
** of the original author. You can produce commercial executables with
** WFC but you can't sell WFC.
**
** Copyright, 1998, Samuel R. Blackburn
**
** $Workfile: CDataLocation.hpp $
** $Revision: 3 $
** $Modtime: 3/08/98 12:03p $
*/

#define DATA_PARSER_CLASS_HEADER

class CDataParser
{
   protected:

      bool         m_AutomaticallyDelete;
      std::vector<uint8_t> * m_Data;

   public:

      CDataParser(const CDataParser&) = delete;
      CDataParser& operator=(const CDataParser&) = delete;

      CDataParser();
      virtual ~CDataParser();

      virtual BOOL Initialize( DWORD& parse_point, std::vector<uint8_t> * data, bool autodelete = false );
      virtual void Empty( void );
};

#endif // DATA_PARSER_CLASS_HEADER
