#if ! defined( WFC_COVERAGE_HEADER_FILE )

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
** Copyright, 1999-2002, Samuel R. Blackburn
**
** $Workfile: wfc_coverage.h $
** $Revision: 1 $
** $Modtime: 7/15/99 7:00a $
*/

#define WFC_COVERAGE_HEADER_FILE

#if defined( WFC_COVERAGE_ANALYSIS )

// wfc.h already defines these for us so we must undefine them

#undef WFC_COVERAGE_DECLARE
#undef WFC_COVERAGE
#undef WFC_COVERAGE_WITH_NAME
#undef WFC_COVERAGE_REPORT
#undef WFC_COVERAGE_SET_NUMBER_OF_POINTS
#undef WFC_COVERAGE_EXTERN_DECLARE
#undef WFC_COVERAGE_RECORD_TIME
#undef WFC_SET_COVERAGE_POINT_NAME

#if ! defined( WFC_NUMBER_OF_COVERAGE_POINTS )
#error "You must define WFC_NUMBER_OF_COVERAGE_POINTS before including wfc_coverage.h"
#endif // WFC_NUMBER_OF_COVERAGE_POINTS

#define WFC_COVERAGE_EXTERN_DECLARE extern unsigned long __wfc_call_count[ WFC_NUMBER_OF_COVERAGE_POINTS ];\
   extern int64_t __wfc_time_count[ WFC_NUMBER_OF_COVERAGE_POINTS ], __wfc_api_overhead_in_counts;\
   extern LPCTSTR __wfc_names[ WFC_NUMBER_OF_COVERAGE_POINTS ]

#define WFC_COVERAGE_DECLARE unsigned long __wfc_call_count[ WFC_NUMBER_OF_COVERAGE_POINTS ];\
   int64_t __wfc_time_count[ WFC_NUMBER_OF_COVERAGE_POINTS ], __wfc_api_overhead_in_counts = 0;\
   LPCTSTR __wfc_names[ WFC_NUMBER_OF_COVERAGE_POINTS ]; \
static void __wfc_coverage_report( void )\
{ \
   LARGE_INTEGER number_of_ticks_per_second = { 0, 0 }; \
   QueryPerformanceFrequency( &number_of_ticks_per_second ); \
   const double number_of_ticks_per_microsecond = (double) ( (double) number_of_ticks_per_second.QuadPart / (double) NUMBER_OF_MICROSECONDS_IN_ONE_SECOND ); \
   const double number_of_ticks_per_millisecond = (double) ( (double) number_of_ticks_per_second.QuadPart / (double) NUMBER_OF_MILLISECONDS_IN_ONE_SECOND ); \
   unsigned long loop_index = 0; \
   CString debug_string; \
   debug_string.Format( TEXT( "Number of counts per second %I64d\nAPI overhead is %I64d ticks.\n" ), \
   number_of_ticks_per_second.QuadPart, __wfc_api_overhead_in_counts ); \
   OutputDebugString( debug_string ); _tprintf( TEXT( "%s" ), (LPCTSTR) debug_string ); \
   int64_t number_of_microseconds = 0; \
   double number_of_milliseconds = 0.0; \
   double number_of_seconds = 0.0; \
   while( loop_index < WFC_NUMBER_OF_COVERAGE_POINTS ) \
   { \
      number_of_seconds = (double) ( (double) __wfc_time_count[ loop_index ] / (double) number_of_ticks_per_second.QuadPart ); \
      number_of_milliseconds = (double) ( (double) __wfc_time_count[ loop_index ] / (double) number_of_ticks_per_millisecond ); \
      number_of_microseconds = (double) ( (double) __wfc_time_count[ loop_index ] / (double) number_of_ticks_per_microsecond ); \
      debug_string.Format( TEXT( "%lu. %s - %lu (%I64dus, %.2lfms, %.4lfs)\n" ), \
         loop_index, \
         __wfc_names[ loop_index ] == NULL ? TEXT( "" ) : __wfc_names[ loop_index ], \
         __wfc_call_count[ loop_index ], number_of_microseconds, number_of_milliseconds, number_of_seconds ); \
      OutputDebugString( debug_string ); _tprintf( TEXT( "%s" ), (LPCTSTR) debug_string ); \
      loop_index++; \
   } }

#define WFC_COVERAGE( x ) __wfc_call_count[ x ]++; \
   const DWORD __wfc_coverage_index = x; \
   LARGE_INTEGER __wfc_coverage_time_start; ::QueryPerformanceCounter( &__wfc_coverage_time_start )

#define WFC_COVERAGE_WITH_NAME( x, y ) __wfc_call_count[ x ]++; __wfc_names[ x ] = y; \
   const DWORD __wfc_coverage_index = x; \
   LARGE_INTEGER __wfc_coverage_time_start; ::QueryPerformanceCounter( &__wfc_coverage_time_start )

#define WFC_SET_COVERAGE_POINT_NAME( x, y ) __wfc_names[ x ] = y

#define WFC_COVERAGE_REPORT() __wfc_coverage_report()

#define WFC_COVERAGE_SET_NUMBER_OF_POINTS  ZeroMemory( __wfc_call_count, sizeof( __wfc_call_count ) ); ZeroMemory( __wfc_time_count, sizeof( __wfc_time_count ) ); ZeroMemory( __wfc_names, sizeof( __wfc_names ) ); \
   LARGE_INTEGER __wfc_coverage_overhead_start; LARGE_INTEGER __wfc_coverage_overhead_stop; Sleep( 0 ); \
   QueryPerformanceCounter( &__wfc_coverage_overhead_start ); \
   QueryPerformanceCounter( &__wfc_coverage_overhead_stop ); \
   __wfc_api_overhead_in_counts = __wfc_coverage_overhead_stop.QuadPart - __wfc_coverage_overhead_start.QuadPart

#define WFC_COVERAGE_RECORD_TIME LARGE_INTEGER __wfc_coverage_time_stop; QueryPerformanceCounter( &__wfc_coverage_time_stop ); \
   __wfc_time_count[ __wfc_coverage_index ] += ((__wfc_coverage_time_stop.QuadPart - __wfc_coverage_time_start.QuadPart ) - __wfc_api_overhead_in_counts )

#endif // WFC_COVERAGE_ANALYSIS

#endif // WFC_COVERAGE_HEADER_FILE
