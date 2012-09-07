#ifndef YTL_ASSEMBLER_EXCEPTION_HPP
#define YTL_ASSEMBLER_EXCEPTION_HPP

#include <exception>

namespace ytl
{
	namespace assembler
	{
		class no_instruction
			: public std::exception
		{
			no_instruction( char const* msg = "no_instruction") throw ()
				: std::exception( msg )
			{}

		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_EXCEPTION_HPP*/