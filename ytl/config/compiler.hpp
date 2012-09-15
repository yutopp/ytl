#ifndef YTL_CONFIG_COMPILER_HPP
#define YTL_CONFIG_COMPILER_HPP

#if defined(_MSC_VER)
#define YTL_C_MSVC
#elif defined(__GNUC__)
#define YTL_C_GCC
#elif defined(__clang__)
#define YTL_C_CLANG
#else
#define YTL_C_OTHER
#endif

#endif /*YTL_CONFIG_COMPILER_HPP*/