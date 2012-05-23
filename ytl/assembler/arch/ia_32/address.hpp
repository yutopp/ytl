#ifndef YTL_ASSEMBLER_IA32_ADDRESS_HPP
#define YTL_ASSEMBLER_IA32_ADDRESS_HPP

#include <boost/variant.hpp>

#include "../../config.hpp"

#include "data_types.hpp"
#include "registers.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace ia_32
		{
			/*
				displacement value
			*/
			template<typename ValueT>
			struct disp_value
			{
				typedef ValueT	value_type;

				explicit disp_value( value_type disp )
					: reg_( registers::r32_t( 4 ) /* 0b100 : [--][--] SIB */ )
					, value_( disp ) {}

				disp_value( registers::r32_t reg, value_type disp )
					: reg_( reg )
					, value_( disp ) {}

				registers::r32_t reg_;		// memorize target register
				value_type value_;
			};

			typedef disp_value<byte_value>	disp8_value;	// 8bit displacement
			typedef disp_value<word_value>	disp16_value;	// 16bit displacement
			typedef disp_value<dword_value>	disp32_value;	// 32bit displacement


			/*
			*/
			struct sib_value
			{
				sib_value(
					byte_t ss,
					registers::r32_t index,
					registers::r32_t base = registers::r32_t( 4 ) /* 0b100 : none */
					)
					: ss_( ss )
					, index_( index )
					, base_( base )
				{}

				byte_t ss_;
				registers::r32_t index_, base_;
			};


			/*
			*/
			template<typename ValueT>
			struct sib_disp_value
			{
				typedef ValueT	value_type;

				sib_disp_value( sib_value const& sib, value_type disp )
					: sib_( sib )
					, disp_( disp ) {}

				sib_value sib_;
				value_type disp_;
			};
			typedef sib_disp_value<byte_value>	sib_disp8_value;	// sib & 8bit displacement
			typedef sib_disp_value<dword_value>	sib_disp32_value;	// sib & 32bit displacement


			namespace registers
			{
				/*
					disp scales : REG32 +- disp[8/32]
				*/
				disp8_value operator+( registers::r32_t const& reg, byte_value disp )
				{
					return disp8_value( reg, disp );
				}
				disp8_value operator-( registers::r32_t const& reg, byte_value disp )
				{
					return disp8_value( reg, byte_value( ~disp.value_ + 1 ) );
				}
			
				disp32_value operator+( registers::r32_t const& reg, int32_t disp )
				{
					return disp32_value( reg, disp );
				}
				disp32_value operator-( registers::r32_t const& reg, int32_t disp )
				{
					return disp32_value( reg, ~disp + 1 );
				}


				/*
					sib & disp scales(Mod R/M[R/M 0b100]) : [--][--] +- disp[8/32]
				*/
				sib_disp8_value operator+( sib_value const& sib, byte_value disp )
				{
					return sib_disp8_value( sib, disp );
				}
				sib_disp8_value operator-( sib_value const& sib, byte_value disp )
				{
					return sib_disp8_value( sib, byte_value( ~disp.value_ + 1 ) );
				}
			
				sib_disp32_value operator+( sib_value const& sib, int32_t disp )
				{
					return sib_disp32_value( sib, disp );
				}
				sib_disp32_value operator-( sib_value const& sib, int32_t disp )
				{
					return sib_disp32_value( sib, ~disp + 1 );
				}


				/* ----------------
				--	sib bites appear only in 32bit addresing format.
				-----------------*/
				/*
					SIB scales(No Base) : REG32 * n
				*/
				sib_value operator*( registers::r32_t const& reg, int32_t scale )	
				{
					assert( reg != registers::esp );
					assert( scale == 0 || scale == 1 || scale == 2 || scale == 4 );

					return sib_value( scale==0 ? 0 : scale==1 ? 1 : scale==2 ? 2 : scale==4 ? 3 : throw/*unreachable*/, reg );
				}
				sib_value operator*( int32_t scale, registers::r32_t const& reg )	
				{
					return reg * scale;
				}
				/*
					SIB scales : BASE_REG32 + ( REG32 * n )[SIB scales(No Base)]
				*/
				sib_value operator+( registers::r32_t const& base, sib_value const& sib )	
				{
					return sib_value( sib.ss_, sib.index_, base );
				}
			} // namespace registers


			/*
			*/
			template<unsigned int N>
			struct mod_rm
			{
				mod_rm( byte_t mod, byte_t rm )
					: mod_( mod )
					, rm_( rm )
				{}

				mod_rm( registers::detail::id<N> reg ) // implicit
					: mod_( 3/*0b11*/ )
					, rm_( reg.get() )
				{}

				byte_t mod_, rm_;	//reg or opcode field id determined later.
			};
			typedef mod_rm<8>	mod_rm8;	// 
			typedef mod_rm<16>	mod_rm16;	// 
			typedef mod_rm<32>	mod_rm32;	// 


			template<unsigned int N, typename DispT>
			struct mod_rm_disp
				: public mod_rm<N>
			{
				typedef typename DispT::value_type value_type;

				mod_rm_disp( byte_t mod, DispT const& disp )
					: mod_rm( mod, disp.reg_.get() )
					, disp_( disp.value_ )
				{}
	
				mod_rm_disp( byte_t mod, byte_t rm, value_type disp_value )
					: mod_rm( mod, rm )
					, disp_( disp_value )
				{}

				value_type disp_;
			};
			typedef mod_rm_disp<8, disp8_value>		mod_rm_disp8;
			typedef mod_rm_disp<16, disp16_value>	mod_rm_disp16;

			typedef mod_rm_disp<32, disp8_value>	mod_rm32_disp8;
			typedef mod_rm_disp<32, disp32_value>	mod_rm32_disp32;


			/* ----------------
			--	sib bites appear only in 32bit addresing format.
			-----------------*/
			struct mod_rm32_sib
				: public mod_rm<32>
			{
				mod_rm32_sib( byte_t mod, byte_t rm, sib_value const& sib )
					: mod_rm( mod, rm )
					, sib_( sib )
				{}

				sib_value sib_;
			};

			template<typename DispT>
			struct mod_rm32_sib_disp
				: public mod_rm<32>
			{
				typedef sib_disp_value<typename DispT::value_type> value_type;
			
				mod_rm32_sib_disp( byte_t mod, byte_t rm, value_type const& disp_value )
					: mod_rm( mod, rm )
					, sib_disp_( disp_value )
				{}

				value_type sib_disp_;
			};
			typedef mod_rm32_sib_disp<disp8_value>	mod_rm32_sib_disp8;
			typedef mod_rm32_sib_disp<disp32_value>	mod_rm32_sib_disp32;

			/* ----------------
			--	Mod R/M
			-----------------*/
/*			typedef boost::variant<
				mod_rm32,			// only mod_rm field
				mod_rm32_disp8,		// mod_rm & disp(1Byte) field
				mod_rm32_disp32,	// mod_rm & disp(4Byte) field
				mod_rm_sib,			// mod_rm & sib(1Byte) field
				mod_rm_sib_disp8,	// mod_rm & sib(1Byte) field & disp(1Byte) field
				mod_rm_sib_disp32	// mod_rm & sib(1Byte) field & disp(4Byte) field
			> addressing;*/

			typedef boost::variant<
				mod_rm8//,			// only mod_rm field
//				mod_rm32_disp8,		// mod_rm & disp(1Byte) field
//				mod_rm32_disp32,	// mod_rm & disp(4Byte) field
			> r_m8_addressing;

			typedef boost::variant<
				mod_rm16//,			// only mod_rm field
//				mod_rm32_disp8,		// mod_rm & disp(1Byte) field
//				mod_rm32_disp32,	// mod_rm & disp(4Byte) field
			> r_m16_addressing;

			typedef boost::variant<
				mod_rm32,			// only mod_rm field
				mod_rm32_disp8,		// mod_rm & disp(1Byte) field
				mod_rm32_disp32,	// mod_rm & disp(4Byte) field
				mod_rm32_sib,		// mod_rm & sib(1Byte) field
				mod_rm32_sib_disp8,	// mod_rm & sib(1Byte) field & disp(1Byte) field
				mod_rm32_sib_disp32	// mod_rm & sib(1Byte) field & disp(4Byte) field
			> r_m32_addressing;

			/*
			*/
			typedef detail::value_wrapper<r_m8_addressing>		r_m8;
			typedef detail::value_wrapper<r_m16_addressing>		r_m16;
			typedef detail::value_wrapper<r_m32_addressing>		r_m32;

			template<unsigned int N, typename T>
			struct implicit_r_m
				: public detail::value_wrapper<T>
			{
				implicit_r_m( T value )	// implicit
					: value_wrapper( value )
				{}

				implicit_r_m( detail::value_wrapper<T> value )
					: value_wrapper( value )
				{}

				implicit_r_m( registers::detail::id<N> reg )
					: value_wrapper( mod_rm<N>( reg ) )
				{}
			};
			typedef implicit_r_m<8, r_m8_addressing>	r_m8_implicit;
			typedef implicit_r_m<16, r_m16_addressing>	r_m16_implicit;
			typedef implicit_r_m<32, r_m32_addressing>	r_m32_implicit;


			namespace detail
			{
				inline byte_t make_mod_rm( byte_t const mod, byte_t const op_code, byte_t const rm )
				{
					return ( mod << 6 & 0xc0 ) | ( op_code << 3 & 0x38 ) | ( rm & 0x07 );
				}

				inline byte_t make_mod_rm( byte_t const op_code, byte_t const rm )
				{
					return make_mod_rm( 3/*0b11*/, op_code, rm );
				}

				template<unsigned int N>
				inline byte_t make_mod_rm( mod_rm<N> const& mod_rm_value, registers::detail::id<N> const& reg )
				{
					return make_mod_rm( mod_rm_value.mod_, reg, mod_rm_value.rm_ );
				}

				inline byte_t make_sib( byte_t const ss, registers::r32_t const index, registers::r32_t const& base )
				{
					return ( ss << 6 & 0xc0 ) | ( index.get() << 3 & 0x38 ) | ( base.get() & 0x07 );
				}
			} // namespace detail

		} // namespace ia_32
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_IA32_ADDRESS_HPP*/