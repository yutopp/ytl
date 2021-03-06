#ifndef YTL_ASSEMBLER_IA32_REGISTERS_HPP
#define YTL_ASSEMBLER_IA32_REGISTERS_HPP

#include "../../config.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace ia_32
		{
			namespace registers
			{
				namespace detail
				{
					template<unsigned int N>
					class id
					{
					public:
						id() {} // todo : make indetermine state.

						explicit id( byte_t v )
							: v_( v ) {}

						bool operator==( id const& rhs ) const
						{
							return v_ == rhs.v_;
						}

						bool operator!=( id const& rhs ) const
						{
							return !( *this == rhs );
						}

						friend byte_t operator+( int lhs, id const& rhs )
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
				} // namespace detail


				// 8bit register
				typedef detail::id<8> r8_t;
				static const r8_t
					al( 0 ),
					cl( 1 ),
					dl( 2 ),
					bl( 3 ),
					ah( 4 ),
					ch( 5 ),
					dh( 6 ),
					bh( 7 );


				// 16bit register
				typedef detail::id<16> r16_t;
				static const r16_t
					ax( 0 ),
					cx( 1 ),
					dx( 2 ),
					bx( 3 ),
					sp( 4 ),
					bp( 5 ),
					si( 6 ),
					di( 7 );


				// 16bit segment register
				typedef detail::id<1600> es_t;
				static const es_t es( 0 );

				typedef detail::id<1601> cs_t;
				static const cs_t cs( 1 );

				typedef detail::id<1602> ss_t;
				static const ss_t ss( 2 );

				typedef detail::id<1603> ds_t;
				static const ds_t ds( 3 );

				typedef detail::id<1604> fs_t;
				static const fs_t fs( 4 );

				typedef detail::id<1605> gs_t;
				static const gs_t gs( 5 );


				// 32bit register
				typedef detail::id<32> r32_t;
				static const r32_t
					eax( 0 ),
					ecx( 1 ),
					edx( 2 ),
					ebx( 3 ),
					esp( 4 ),
					ebp( 5 ),
					esi( 6 ),
					edi( 7 );

			} // namespace registers
		} // namespece ia_32
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_IA32_REGISTERS_HPP*/
