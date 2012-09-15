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
		void read_binary_impl( CharT const* const filename, Buffer& dst, typename std::enable_if<!has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			typedef typename Buffer::value_type value_type;

			std::cout << sizeof( value_type ) << std::endl;

			std::basic_ifstream<value_type> ifs( filename, std::ios::binary );
			if ( !ifs )
				throw std::exception( "File no exists." );

			std::istreambuf_iterator<value_type> const begin( ifs ), end;
			std::copy( begin, end, std::back_inserter( dst ) );
		}

		template<typename Buffer, typename CharT>
		inline void read_binary_impl( CharT const* const filename, Buffer& dst, typename std::enable_if<has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			read_binary_impl( filename, (*dst) );
		}
	} // namespace detail

	template<typename Buffer, typename CharT>
	inline void read_binary( CharT const* const filename, Buffer& dst )
	{
		detail::read_binary_impl( filename, dst );
	}

	template<typename Buffer, typename CharT>
	inline void read_binary( std::basic_string<CharT> const& filename, Buffer& dst )
	{
		read_binary( filename.c_str(), dst );
	}

	template<typename Buffer, typename CharT>
	Buffer read_binary( CharT const* const filename )
	{
		Buffer h;
		read_binary( filename, h );

		return h;
	}

} // namespace ytl

#endif /*YTL_UTILITY_READ_BINARY_HPP*/