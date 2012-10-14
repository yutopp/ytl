#ifndef YTL_UTILITY_GUARD_MACROS_HPP
#define YTL_UTILITY_GUARD_MACROS_HPP


#define YTL_REQUIRE_BINARY_BUFFER( Container )				\
	static_assert(											\
		sizeof( typename Container::value_type ) == 1,	\
		__FILE__ " required element of buffer is 1."		\
		);


#endif /*YTL_UTILITY_GUARD_MACROS_HPP*/