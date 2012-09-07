#ifndef YTL_UTILITY_READ_BINARY_HPP
#define YTL_UTILITY_READ_BINARY_HPP

#include <string>
#include <fstream>
#include <memory>
#include <iterator>
#include <algorithm>

#include <boost/mpl/has_xxx.hpp>

#include <ytl/config.hpp>

#include "has_wrapped_container_type.hpp"

namespace ytl
{
	namespace detail
	{
		template<typename Buffer, typename CharT>
		void read_binary_impl( Buffer& dst, CharT const* const filename, typename std::enable_if<!has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			typedef typename Buffer::value_type value_type;

			std::basic_ifstream<value_type> ifs( filename, std::ios::binary );
			if ( !ifs )
				throw std::exception( "" );

			std::istreambuf_iterator<value_type> const begin( ifs ), end;
			std::copy( begin, end, std::back_inserter( binary_ ) );
		}

		template<typename Buffer, typename CharT>
		inline void read_binary_impl( Buffer& dst, CharT const* const filename, typename std::enable_if<has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			read_binary_impl( (*dst), filename );
		}
	} // namespace detail

	template<typename Buffer, typename CharT>
	inline void read_binary( Buffer& dst, CharT const* const filename )
	{
		detail::read_binary_impl( dst, filename );
	}

	template<typename Buffer, typename CharT>
	inline void read_binary( Buffer& dst, std::basic_string<CharT> const& filename )
	{
		read_binary( dst, filename.c_str() );
	}

	template<typename Buffer, typename CharT>
	Buffer read_binary( CharT const* const filename )
	{
		Buffer h;
		read_binary( h, filename );

		return h;
	}

} // namespace ytl

#endif /*YTL_UTILITY_READ_BINARY_HPP*/