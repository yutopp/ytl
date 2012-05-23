#ifndef YTL_ASSEMBLER_EXCEPTION_HPP
#define YTL_ASSEMBLER_EXCEPTION_HPP

#include <exception>

namespace ytl
{
	namespace assembler
	{
		class no_operator
			: public std::exception
		{
			no_operator( char const* msg = "no_operator") throw ()
				: std::exception( msg )
			{}

		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_EXCEPTION_HPP*/