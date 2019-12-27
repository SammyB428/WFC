#if ! defined( STL_STRING_HELPERS_HEADER_FILE )

#define STL_STRING_HELPERS_HEADER_FILE

#define STRING_VIEW(_x)  std::string_view(  _x, std::size(_x) - 1)
#define WSTRING_VIEW(_x) std::wstring_view( _x, std::size(_x) - 1)

#include "stl_format.h"
#include "stl_append.h"
#include "stl_make.h"
#include "stl_compare.h"
#include "stl_left_right.h"
#include "stl_safe.h"
#include "stl_is.h"
#include "stl_unique.h"
#include "stl_trim.h"
#include "stl_copy.h"
#include "stl_with.h"
#include "stl_split_join.h"
#include "stl_convert.h"
#include "stl_insert_remove.h"

#endif // STL_STRING_HELPERS_HEADER_FILE