#ifndef YTL_FILEFORMAT_UTILITY_HPP
#define YTL_FILEFORMAT_UTILITY_HPP

#include <string>
#include <utility>

#include <ytl/utility.hpp>

namespace ytl
{
	namespace fileformat
	{
		template<typename FileT, typename CharT>
		FileT read( CharT const* const filename )
		{
			typedef typename std::remove_cv<FileT>::type object_type;
			typedef typename std::add_pointer<object_type>::type pointer_t;

			return build_object<object_type>( filename, static_cast<pointer_t>( nullptr ) );
		}

		template<typename FileT, typename CharT>
		FileT read( std::basic_string<CharT> const& filename )
		{
			return read<FileT>( filename.c_str() );
		}


		// generic
		template<typename T, typename CharT>
		T build_object( CharT const* const filename, void* )
		{
			auto bin = read_binary<T>( filename );
			
		}

	} // namespace fireformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_UTILITY_HPP*/