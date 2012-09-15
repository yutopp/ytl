#ifndef YTL_ASSEMBLER_VALIE_WRAPPER_HPP
#define YTL_ASSEMBLER_VALIE_WRAPPER_HPP

#include "../../config.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace detail
		{
			template<typename T>
			struct value_wrapper
			{
				typedef T value_type;

				explicit value_wrapper( value_type const& value )
					: value_( value )
				{}

				operator value_type() const
				{
					return value_;
				}

				value_type value_;
			};

		} // namespace detail
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_VALIE_WRAPPER_HPP*/