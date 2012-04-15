#ifndef YTL_ASSEMBLER_IA32_REGISTERS_HPP
#define YTL_ASSEMBLER_IA32_REGISTERS_HPP

#include "../../config.hpp"

namespace ytd
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

						operator byte_t() const { return v_; }

					private:
						byte_t v_;
					};
				} // namespace detail


				// 8bit register
				static const detail::id<8>
					al( 0 ),
					cl( 1 ),
					dl( 2 ),
					bl( 3 ),
					ah( 4 ),
					ch( 5 ),
					dh( 6 ),
					bh( 7 );


				// 16bit register
				static const detail::id<16>
					ax( 0 ),
					cx( 1 ),
					dx( 2 ),
					bx( 3 ),
					sp( 4 ),
					bp( 5 ),
					si( 6 ),
					di( 7 );


				// 32bit register
				static const detail::id<32>
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
} // namespace ytd

#endif /*YTL_ASSEMBLER_IA32_REGISTERS_HPP*/
