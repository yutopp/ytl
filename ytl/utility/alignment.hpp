#ifndef YTL_UTILITY_ALIGNMENT_HPP
#define YTL_UTILITY_ALIGNMENT_HPP

#include <ytl/config.hpp>

#if defined(YTL_C_MSVC)

#define YTL_ALIGN( N ) __declspec( align( N ) )

#elif defined(YTL_C_GCC)

#define YTL_ALIGN( N ) __attribute__ ((aligned( N )))

#else

#error Not supported...

#endif


#endif /*YTL_UTILITY_ALIGNMENT_HPP*/