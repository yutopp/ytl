#ifndef YTL_UTILITY_READ_BINARY_FILE_HPP
#define YTL_UTILITY_READ_BINARY_FILE_HPP

#include <memory>

#include <ytl/buffer/binary_buffer.hpp>

#include "read_binary_file_core.hpp"

namespace ytl
{
	// type specified
	template<typename CharT>
	inline binary_buffer read( CharT const* const filename )
	{
		return read<binary_buffer<>>( filename );
	}

	template<typename CharT>
	inline binary_buffer read( std::basic_string<CharT> const& filename )
	{
		return read<binary_buffer>( filename.c_str() );
	}


	// shared_type
	template<typename Buffer, typename CharT>
	std::shared_ptr<Buffer> read_shared( CharT const* const filename )
	{
		std::shared_ptr<Buffer> p( std::make_shared<Buffer>() );
		read( filename, *p );

		return p;
	}

	template<typename Buffer, typename CharT>
	inline std::shared_ptr<Buffer> read_shared( std::basic_string<CharT> const& filename )
	{
		return read_shared<Buffer>( filename.c_str() );
	}

	template<typename CharT>
	inline std::shared_ptr<binary_buffer> read_shared( CharT const* const filename )
	{
		return read_shared<binary_buffer>( filename );
	}

	template<typename CharT>
	inline std::shared_ptr<binary_buffer> read_shared( std::basic_string<CharT> const& filename )
	{
		return read_shared<binary_buffer>( filename.c_str() );
	}



	// unique_type
	template<typename Buffer, typename CharT>
	std::unique_ptr<Buffer> read_unique( CharT const* const filename )
	{
		std::unique_ptr<Buffer> p( new Buffer() );
		read( filename, *p );

		return p;
	}

	template<typename Buffer, typename CharT>
	inline std::unique_ptr<Buffer> read_unique( std::basic_string<CharT> const& filename )
	{
		return read_unique<Buffer>( filename.c_str() );
	}

	template<typename CharT>
	inline std::unique_ptr<binary_buffer> read_unique( CharT const* const filename )
	{
		return read_unique<binary_buffer>( filename );
	}

	template<typename CharT>
	inline std::unique_ptr<binary_buffer> read_unique( std::basic_string<CharT> const& filename )
	{
		return read_unique<binary_buffer>( filename.c_str() );
	}

} // namespace ytl

#endif /*YTL_UTILITY_READ_BINARY_FILE_HPP*/