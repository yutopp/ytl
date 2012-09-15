#ifndef YTL_UTILITY_READ_BINARY_HPP
#define YTL_UTILITY_READ_BINARY_HPP

#include <string>
#include <fstream>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#include <boost/mpl/has_xxx.hpp>

#include <ytl/config.hpp>
#include <ytl/buffer/binary_buffer.hpp>

namespace ytl
{
	template<typename Buffer, typename CharT>
	inline void read_binary( CharT const* const filename, Buffer& dst )
	{
		typedef typename Buffer::value_type value_type;

		std::basic_ifstream<byte_t> ifs( filename, std::ios_base::in | std::ios::binary );
		if ( !ifs )
			throw std::runtime_error( "File no exists." );

		std::istreambuf_iterator<byte_t> const begin( ifs ), end;
		std::copy( begin, end, std::back_inserter( dst ) );
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

	template<typename Buffer, typename CharT>
	inline Buffer read_binary( std::basic_string<CharT> const& filename )
	{
		return read_binary<Buffer>( filename.c_str() );
	}

	template<typename CharT>
	inline binary_buffer<> read_binary( CharT const* const filename )
	{
		return read_binary<binary_buffer<>>( filename );
	}

	template<typename CharT>
	inline binary_buffer<> read_binary( std::basic_string<CharT> const& filename )
	{
		return read_binary<binary_buffer<>>( filename.c_str() );
	}


	template<typename Buffer, typename CharT>
	std::shared_ptr<Buffer> read_binary_as_shared( CharT const* const filename )
	{
		std::shared_ptr<Buffer> h = std::make_shared<Buffer>();
		read_binary( filename, *h );

		return h;
	}

	template<typename Buffer, typename CharT>
	inline std::shared_ptr<Buffer>
		read_read_binary_as_sharedbinary( std::basic_string<CharT> const& filename )
	{
		return read_binary_as_shared<Buffer>( filename.c_str() );
	}

	template<typename CharT>
	inline std::shared_ptr<binary_buffer<>>
		read_binary_as_shared( CharT const* const filename )
	{
		return read_binary_as_shared<binary_buffer<>>( filename );
	}

	template<typename CharT>
	inline std::shared_ptr<binary_buffer<>>
		read_binary_as_shared( std::basic_string<CharT> const& filename )
	{
		return read_binary_as_shared<binary_buffer<>>( filename.c_str() );
	}
} // namespace ytl

#endif /*YTL_UTILITY_READ_BINARY_HPP*/