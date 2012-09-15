#ifndef YTL_ASSEMBLER_REGISTER_ID_HPP
#define YTL_ASSEMBLER_REGISTER_ID_HPP

#include "../../config.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace detail
		{
			template<unsigned int N>
			class register_id
			{
				typedef register_id		self_type;
			public:
				register_id() {} // todo : make indetermine state.

				explicit register_id( byte_t const v )
					: v_( v ) {}

				bool operator==( self_type const& rhs ) const
				{
					return v_ == rhs.v_;
				}

				bool operator!=( self_type const& rhs ) const
				{
					return !( *this == rhs );
				}

				friend byte_t operator+( int const lhs, self_type const& rhs )
				{
					return lhs + rhs.v_;
				}

				byte_t get() const
				{
					return v_;
				}

			private:
				byte_t v_;
			};

		} // namespece detail
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_REGISTER_ID_HPP*/
