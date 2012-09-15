#include <ytl/config/compiler.hpp>

#if defined(YTL_C_MSVC)

#pragma pack( pop )

#elif defined(YTL_C_GCC)

#pragma pack( 0 )

#else

#error Not supported...

#endif