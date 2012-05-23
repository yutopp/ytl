#ifndef YTL_ASSEMBLER_ENDIAN_WRITER_HPP
#define YTL_ASSEMBLER_ENDIAN_WRITER_HPP

#include "detail/little_endian_writer.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace endian
		{
			typedef detail::little_endian_writer	little;

		} // namespace endian
	} // namespace assembeler
} // namespace ytl

#endif /*YTL_ASSEMBLER_ENDIAN_WRITER_HPP*/