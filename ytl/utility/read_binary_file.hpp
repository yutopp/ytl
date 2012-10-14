#ifndef YTL_UTILITY_READ_BINARY_FILE_HPP
#define YTL_UTILITY_READ_BINARY_FILE_HPP

#include <memory>

#include <ytl/buffer/binary_buffer.hpp>

#include "read_binary_file_core.hpp"

namespace ytl
{
	// type specified
	template<typename CharT>
	inline binary_buffer<> read_binary_file( CharT const* const filename )
	{
		return read_binary_file<binary_buffer<>>( filename );
	}

	template<typename CharT>
	inline binary_buffer<> read_binary_file( std::basic_string<CharT> const& filename )
	{
		return read_binary_file<binary_buffer<>>( filename.c_str() );
	}


	// shared_type
	template<typename Buffer, typename CharT>
	std::shared_ptr<Buffer> read_shared_binary_file( CharT const* const filename )
	{
		std::shared_ptr<Buffer> p( std::make_shared<Buffer>() );
		read_binary_file( filename, *p );

		return p;
	}

	template<typename Buffer, typename CharT>
	inline std::shared_ptr<Buffer> read_shared_binary_file( std::basic_string<CharT> const& filename )
	{
		return read_shared_binary_file<Buffer>( filename.c_str() );
	}

	template<typename CharT>
	inline std::shared_ptr<binary_buffer<>> read_shared_binary_file( CharT const* const filename )
	{
		return read_shared_binary_file<binary_buffer<>>( filename );
	}

	template<typename CharT>
	inline std::shared_ptr<binary_buffer<>> read_shared_binary_file( std::basic_string<CharT> const& filename )
	{
		return read_shared_binary_file<binary_buffer<>>( filename.c_str() );
	}



	// unique_type
	template<typename Buffer, typename CharT>
	std::unique_ptr<Buffer> read_unique_binary_file( CharT const* const filename )
	{
		std::unique_ptr<Buffer> p( new Buffer() );
		read_binary_file( filename, *p );

		return p;
	}

	template<typename Buffer, typename CharT>
	inline std::unique_ptr<Buffer> read_unique_binary_file( std::basic_string<CharT> const& filename )
	{
		return read_unique_binary_file<Buffer>( filename.c_str() );
	}

	template<typename CharT>
	inline std::unique_ptr<binary_buffer<>> read_unique_binary_file( CharT const* const filename )
	{
		return read_unique_binary_file<binary_buffer<>>( filename );
	}

	template<typename CharT>
	inline std::unique_ptr<binary_buffer<>> read_unique_binary_file( std::basic_string<CharT> const& filename )
	{
		return read_unique_binary_file<binary_buffer<>>( filename.c_str() );
	}

} // namespace ytl

#endif /*YTL_UTILITY_READ_BINARY_FILE_HPP*/