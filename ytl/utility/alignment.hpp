#ifndef YTL_UTILITY_ALIGNMENT_HPP
#define YTL_UTILITY_ALIGNMENT_HPP

#include <ytl/config.hpp>

#if defined( YTL_C_MSVC )

#define YTL_ALIGN( N ) __declspec( align( N ) )

#elif defined( YTL_C_GCC )

#define YTL_ALIGN( N ) __attribute__ ((aligned( N )))

#else

#error Not supported...

#endif

namespace ytl
{
    inline std::size_t padding_size( std::size_t const size, std::size_t alignment )
    {
        return ( size % alignment != 0 )
                ? ( alignment - size % alignment )
                : 0;
    }

    inline std::size_t aligned_size( std::size_t const size, std::size_t alignment )
    {
        return size + padding_size( size, alignment );
    }
}

#endif /*YTL_UTILITY_ALIGNMENT_HPP*/