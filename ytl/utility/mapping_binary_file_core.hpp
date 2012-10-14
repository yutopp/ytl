#ifndef YTL_UTILITY_MAPPING_BINARY_FILE_CORE_HPP
#define YTL_UTILITY_MAPPING_BINARY_FILE_CORE_HPP

#include <ytl/buffer/file_mapped_binary_buffer.hpp>

namespace ytl
{
	template<typename CharT>
	file_mapped_binary_buffer mapping( CharT const* const filename )
	{
		return file_mapped_binary_buffer( filename );
	}

	template<typename Buffer, typename CharT>
	inline file_mapped_binary_buffer mapping( std::basic_string<CharT> const& filename )
	{
		return file_mapped_binary_buffer( filename.c_str() );
	}

} // namespace ytl

#endif /*YTL_UTILITY_MAPPING_BINARY_FILE_CORE_HPP*/
