#ifndef YTL_CONFIG_TYPEDEF_HPP
#define YTL_CONFIG_TYPEDEF_HPP

#include <cstddef>

#include <boost/cstdint.hpp>

namespace ytl
{
	typedef boost::uint_least8_t	byte_t;
	typedef boost::uint_least16_t	word_t;
	typedef	boost::uint_least32_t	dword_t;

	typedef	boost::int8_t			int8_t;
	typedef	boost::int16_t			int16_t;
	typedef	boost::int32_t			int32_t;

} // namespace ytl

#endif /*YTL_CONFIG_TYPEDEF_HPP*/