#ifndef YTL_ASSEMBLER_X86_DATA_TYPE_HPP
#define YTL_ASSEMBLER_X86_DATA_TYPE_HPP

#include "../../config.hpp"
#include "../detail/value_wrapper.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace x86
		{
			/*
				8bit/16bit/32bit value wrappers
			*/
			typedef detail::value_wrapper<byte_t> byte_value;

			typedef detail::value_wrapper<word_t> word_value;

			struct dword_value
				: public detail::value_wrapper<dword_t>
			{
				dword_value( dword_t value )	// implicit
					: value_wrapper( value ) {}

				dword_value( byte_value value )	// implicit
					: value_wrapper( value.value_ ) {}
				dword_value( word_value value )	// implicit
					: value_wrapper( value.value_ ) {}
			};

		} // namespace x86
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_DATA_TYPE_HPP*/