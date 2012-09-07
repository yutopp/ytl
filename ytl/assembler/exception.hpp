#ifndef YTL_ASSEMBLER_EXCEPTION_HPP
#define YTL_ASSEMBLER_EXCEPTION_HPP

#include <stdexcept>

namespace ytl
{
	namespace assembler
	{
		class no_instruction
			: public std::runtime_error
		{
			explicit no_instruction() throw ()
				: std::runtime_error( "no_instruction" )
			{}

		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_EXCEPTION_HPP*/