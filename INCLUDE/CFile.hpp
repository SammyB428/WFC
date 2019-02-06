/*
** Author: Samuel R. Blackburn
**
** $Workfile: CFile.hpp $
** $Revision: 4 $
** $Modtime: 7/23/00 1:11p $
** $Reuse Tracing Code: 1001 $
*/

/*
** NOTICE NOTICE NOTICE
**
** Many of the things you will see in this class go against the coding
** standards as well as good coding practices. The reason for this is we
** are trying to make a CFile drop-in replacement. This means we have to
** minimize the damage a replacement will make. So, we have to duplicate
** the bad habits of Microsoft. Where possible, we will do the right thing.
** One obvious break point will be using ULONGLONG instead of DWORD for
** things like file position.
*/

#if ! defined( FILE_CLASS_HEADER )

#define FILE_CLASS_HEADER

#endif // FILE_CLASS_HEADER
