#include <ytl/config/compiler.hpp>

#if defined( YTL_C_MSVC )

#pragma pack( push, 2 )

#elif defined( YTL_C_GCC )

#pragma pack( 2 )

#else

#error Not supported...

#endif