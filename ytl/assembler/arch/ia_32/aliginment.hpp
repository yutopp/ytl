#ifndef YTL_ASSEMBLER_IA32_ALIGINMENT_HPP
#define YTL_ASSEMBLER_IA32_ALIGINMENT_HPP

#include <cassert>

#include "../../config.hpp"

#include "registers.hpp"
#include "data_types.hpp"
#include "address.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace ia_32
		{
			/*
				alignment annotation
			*/
			namespace alignment
			{
				/*
					32bit pointer annotation
				*/
				static const struct pointer_mod_rm_annotation
				{
					/* ---------
						32bit addresing mode.
					--------- */
					// Mod 0b00
					r_m32 operator[]( registers::r32_t reg ) const
					{
						assert( reg != registers::esp && reg != registers::ebp );
						return r_m32( mod_rm32( 0, reg.get() ) );
					}
					r_m32 operator[]( dword_value disp ) const
					{
						return r_m32( mod_rm32_disp32( 0, 5/*0b101*/, disp ) );
					}
					r_m32 operator[]( sib_value const& sib ) const
					{
						return r_m32( mod_rm32_sib( 0, 4/*0b100*/, sib ) );
					}
				
					// Mod 0b01
					r_m32 operator[]( disp8_value const& disp ) const
					{
						assert( disp.reg_ != registers::esp );
						return r_m32( mod_rm32_disp8( 1/*0b01*/, disp ) );
					}
					r_m32 operator[]( sib_disp8_value const& sib_disp ) const
					{
						assert( sib_disp.sib_.base_ != registers::ebp && sib_disp.sib_.index_ != registers::esp );
						return r_m32( mod_rm32_sib_disp8( 1/*0b01*/, 4/*0b100*/, sib_disp ) );
					}
				
					// Mod 0b10
					r_m32 operator[]( disp32_value const& disp ) const
					{
						assert( disp.reg_ != registers::esp );
						return r_m32( mod_rm32_disp32( 2/*0b10*/, disp ) );
					}

					r_m32 operator[]( sib_disp32_value const& sib_disp ) const
					{
						assert( sib_disp.sib_.base_ != registers::ebp && sib_disp.sib_.index_ != registers::esp );
						return r_m32( mod_rm32_sib_disp32( 2/*0b10*/, 4/*0b100*/, sib_disp ) );
					}

					// Mod 0b11
					// -> see address.hpp class mod_rm's implicit ctor
				} ptr;


				/*
				*/
				static const struct byte_value_annotation
				{
					byte_value operator[]( byte_t value ) const
					{
						return byte_value( value );
					}

/*					template<typename T>
					r_m8 operator[]( detail::value_wrapper<addressing, T> const& r_mN ) const
					{
						return r_m8( r_mN.value_ );
					}*/
				} byte;


				/*
				*/
				static const struct word_value_annotation
				{
					word_value operator[]( word_t value ) const
					{
						return word_value( value );
					}

/*					template<typename T>
					r_m16 operator[]( detail::value_wrapper<addressing, T> const& r_mN ) const
					{
						return r_m16( r_mN.value_ );
					}*/
				} word;


				/*
				*/
				static const struct dword_value_annotation
				{
					dword_value operator[]( dword_t value ) const
					{
						return dword_value( value );
					}

/*					template<typename T>
					r_m32 operator[]( detail::value_wrapper<addressing, T> const& r_mN ) const
					{
						return r_m32( r_mN.value_ );
					}*/
				} dword;

			} // namespace alignment
		} // namespace ia_32
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_IA32_ALIGINMENT_HPP*/
