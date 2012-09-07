#ifndef YTL_UTILITY_MAPPING_BINARY_FILE_HPP
#define YTL_UTILITY_MAPPING_BINARY_FILE_HPP

#include <memory>

#include "mapping_binary_file_core.hpp"

namespace ytl
{
	// shared_type
	template<typename CharT>
	std::shared_ptr<file_mapped_binary_buffer> mapping_shared( CharT const* const filename )
	{
		return std::make_shared<file_mapped_binary_buffer>( filename );
	}

	template<typename CharT>
	inline std::shared_ptr<file_mapped_binary_buffer> mapping_shared( std::basic_string<CharT> const& filename )
	{
		return mapping_shared( filename.c_str() );
	}

	// unique_type
	template<typename CharT>
	std::unique_ptr<file_mapped_binary_buffer> mapping_unique( CharT const* const filename )
	{
		std::unique_ptr<file_mapped_binary_buffer> p( new file_mapped_binary_buffer( filename ) );

		return p;
	}

	template<typename CharT>
	inline std::unique_ptr<file_mapped_binary_buffer> mapping_unique( std::basic_string<CharT> const& filename )
	{
		return mapping_unique( filename.c_str() );
	}

} // namespace ytl

#endif /*YTL_UTILITY_MAPPING_BINARY_FILE_HPP*/