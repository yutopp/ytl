#ifndef YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_WRITER_HPP
#define YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_WRITER_HPP

#include <cstring>

#include "../config.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace detail
		{
			/*
			*/
			struct little_endian_writer
			{
				template<typename T>
				static void write( byte_t* const dst, T const* const src, std::size_t const size )
				{
#ifdef YTL_LITTLE_ENDIAN
					std::memcpy( dst, src, size );
#elif YTL_BIG_ENDIAN

#else
# error ""
#endif
				}
			};

		} // namespace detail
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_WRITER_HPP*/
