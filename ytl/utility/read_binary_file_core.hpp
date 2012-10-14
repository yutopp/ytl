#ifndef YTL_UTILITY_READ_BINARY_FILE_CORE_HPP
#define YTL_UTILITY_READ_BINARY_FILE_CORE_HPP

#include <string>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#include <ytl/config.hpp>

#include "guard_macros.hpp"

namespace ytl
{
	template<typename Buffer, typename CharT>
	void read( CharT const* const filename, Buffer& dst )
	{
		YTL_REQUIRE_BINARY_BUFFER( Buffer )

		typedef typename Buffer::value_type		value_type;

		std::basic_ifstream<char /*byte_t*/> ifs( filename, std::ios_base::in | std::ios::binary );
		if ( !ifs )
			throw std::runtime_error( "File no exists." );

		std::istreambuf_iterator<char /*byte_t*/> const begin( ifs ), end;
		std::copy( begin, end, std::back_inserter( dst ) );
	}

	template<typename Buffer, typename CharT>
	inline void read( std::basic_string<CharT> const& filename, Buffer& dst )
	{
		read( filename.c_str(), dst );
	}


	template<typename Buffer, typename CharT>
	Buffer read( CharT const* const filename )
	{
		Buffer buf;
		read_binary_file( filename, buf );

		return buf;
	}

	template<typename Buffer, typename CharT>
	inline Buffer read( std::basic_string<CharT> const& filename )
	{
		return read<Buffer>( filename.c_str() );
	}

} // namespace ytl

#endif /*YTL_UTILITY_READ_BINARY_FILE_CORE_HPP*/