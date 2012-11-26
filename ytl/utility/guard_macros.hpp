#ifndef YTL_UTILITY_GUARD_MACROS_HPP
#define YTL_UTILITY_GUARD_MACROS_HPP

#include <type_traits>

#define YTL_ENABLE_IF_ONLY_BUFFER_TYPE( Container )                         \
    typename std::add_pointer<                                              \
        typename std::remove_reference<Container>::type::value_type         \
    >::type =0


#define YTL_REQUIRE_BINARY_BUFFER( Container )                              \
    static_assert(                                                          \
        sizeof(                                                             \
            typename std::remove_reference<Container>::type::value_type     \
            ) == 1,                                                         \
        __FILE__ " : Required element size of buffer == 1."                 \
        );


#endif /*YTL_UTILITY_GUARD_MACROS_HPP*/