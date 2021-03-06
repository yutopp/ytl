#ifndef YTL_ASSEMBLER_IA32_DATA_TYPE_HPP
#define YTL_ASSEMBLER_IA32_DATA_TYPE_HPP

#include "../../config.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace ia_32
		{
			namespace detail
			{
				template<typename T>
				struct value_wrapper
				{
					typedef T value_type;

					explicit value_wrapper( value_type const& value )
						: value_( value ) {}

					operator value_type() const { return value_; }

					value_type value_;
				};
			} // namespace detail


			/*
				8bit/16bit/32bit value wrappers
			*/
			typedef detail::value_wrapper<int8_t> byte_value;
			typedef detail::value_wrapper<int16_t> word_value;
			struct dword_value
				: public detail::value_wrapper<int32_t>
			{
				dword_value( dword_t value )	// implicit
					: value_wrapper( value ) {}

				dword_value( byte_value value )	// implicit
					: value_wrapper( value.value_ ) {}
				dword_value( word_value value )	// implicit
					: value_wrapper( value.value_ ) {}
			};

		} // namespace ia_32
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_IA32_DATA_TYPE_HPP*/