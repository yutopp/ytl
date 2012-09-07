#ifndef YTL_ASSEMBLER_X86_REGISTERS_HPP
#define YTL_ASSEMBLER_X86_REGISTERS_HPP

#include "../detail/register_id.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace x86
		{
			namespace registers
			{
				// 8bit register
				typedef detail::register_id<8> r8_t;
				const r8_t
					al( 0 ),
					cl( 1 ),
					dl( 2 ),
					bl( 3 ),
					ah( 4 ),
					ch( 5 ),
					dh( 6 ),
					bh( 7 );


				// 16bit register
				typedef detail::register_id<16> r16_t;
				const r16_t
					ax( 0 ),
					cx( 1 ),
					dx( 2 ),
					bx( 3 ),
					sp( 4 ),
					bp( 5 ),
					si( 6 ),
					di( 7 );


				// 16bit segment register
				typedef detail::register_id<1600> es_t;
				const es_t es( 0 );

				typedef detail::register_id<1601> cs_t;
				const cs_t cs( 1 );

				typedef detail::register_id<1602> ss_t;
				const ss_t ss( 2 );

				typedef detail::register_id<1603> ds_t;
				const ds_t ds( 3 );

				typedef detail::register_id<1604> fs_t;
				const fs_t fs( 4 );

				typedef detail::register_id<1605> gs_t;
				const gs_t gs( 5 );


				// 32bit register
				typedef detail::register_id<32> r32_t;
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
		} // namespece x86
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_REGISTERS_HPP*/
