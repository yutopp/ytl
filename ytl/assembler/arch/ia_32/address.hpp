#ifndef YTL_ASSEMBLER_IA32_ADDRESS_HPP
#define YTL_ASSEMBLER_IA32_ADDRESS_HPP

#include "../../config.hpp"

#include "data_types.hpp"
#include "registers.hpp"

#include <boost/variant.hpp>

namespace ytd
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
					: reg_( registers::detail::id<32>( 4 ) /* 0b100 : [--][--] SIB */ )
					, value_( disp ) {}

				disp_value( registers::detail::id<32> reg, value_type disp )
					: reg_( reg )
					, value_( disp ) {}

				registers::detail::id<32> reg_;		// memorize target register
				value_type value_;
			};

			typedef disp_value<byte_value>	disp8_value;	// 8bit displacement
			typedef disp_value<dword_value>	disp32_value;	// 32bit displacement

			/*
			*/
			struct sib_value
			{
				sib_value(
					byte_t ss,
					registers::detail::id<32> index,
					registers::detail::id<32> base = registers::detail::id<32>( 4 ) /* 0b100 : none */
					)
					: ss_( ss )
					, index_( index )
					, base_( base ) {}

				byte_t ss_;
				registers::detail::id<32> index_, base_;
			};

			/*
			*/
			template<typename ValueT>
			struct sib_disp_value
			{
				typedef ValueT	value_type;

				explicit sib_disp_value( sib_value const& sib, value_type disp )
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
					disp scales : REG +- disp
				*/
				disp8_value operator+( registers::detail::id<32> const& reg, byte_value disp )
				{
					return disp8_value( reg, disp );
				}
				disp8_value operator-( registers::detail::id<32> const& reg, byte_value disp )
				{
					return disp8_value( reg, byte_value( ~disp.value_ + 1 ) );
				}
			
				disp32_value operator+( registers::detail::id<32> const& reg, int disp )
				{
					return disp32_value( reg, disp );
				}
				disp32_value operator-( registers::detail::id<32> const& reg, int disp )
				{
					return disp32_value( reg, ~disp + 1 ); // buggy?
				}

				/*
					sib disp scales : BASE + REG * n +- disp
				*/
				sib_disp8_value operator+( sib_value const& sib, byte_value disp )
				{
					return sib_disp8_value( sib, disp );
				}
				sib_disp8_value operator-( sib_value const& sib, byte_value disp )
				{
					return sib_disp8_value( sib, byte_value( ~disp.value_ + 1 ) );
				}
			
				sib_disp32_value operator+( sib_value const& sib, int disp )
				{
					return sib_disp32_value( sib, disp );
				}
				sib_disp32_value operator-( sib_value const& sib, int disp )
				{
					return sib_disp32_value( sib, ~disp + 1 ); // buggy?
				}

				/*
					SIB scales
				*/
				sib_value operator*( registers::detail::id<32> const& reg, int scale )	// REG * n
				{
					assert( scale == 0 || scale == 1 || scale == 2 || scale == 4 );

					return sib_value( scale==0 ? 0 : scale==1 ? 1 : scale==2 ? 2 : scale==4 ? 3 : throw/*unreachable*/, reg );
				}

				sib_value operator+( registers::detail::id<32> const& base, sib_value const& sib )	// BASE + REG * n
				{
					return sib_value( sib.ss_, sib.index_, base );
				}
			} // namespace registers

			/*
			*/
			struct mod_rm
			{
				mod_rm( byte_t mod, byte_t rm )
					: mod_( mod )
					, rm_( rm )
				{}

				template<unsigned int N>
				mod_rm( registers::detail::id<N> reg ) // implicit
					: mod_( 3/*0b11*/ )
					, rm_( reg )
				{}

				byte_t mod_, rm_;	//reg or opcode field id determined later.
			};


			template<typename DispT>
			struct mod_rm_disp
				: public mod_rm
			{
				typedef typename DispT::value_type value_type;

				mod_rm_disp( byte_t mod, DispT const& disp )
					: mod_rm( mod, disp.reg_ )
					, disp_( disp.value_ )
				{}

				
				mod_rm_disp( byte_t mod, byte_t rm, value_type disp_value )
					: mod_rm( mod, rm )
					, disp_( disp_value )
				{}

				value_type disp_;
			};
			typedef mod_rm_disp<disp8_value>	mod_rm_disp8;
			typedef mod_rm_disp<disp32_value>	mod_rm_disp32;


			struct mod_rm_sib
				: public mod_rm
			{
				mod_rm_sib( byte_t mod, byte_t rm, sib_value const& sib )
					: mod_rm( mod, rm )
					, sib_( sib )
				{}

				sib_value sib_;
			};


			template<typename DispT>
			struct mod_rm_sib_disp
				: public mod_rm
			{
				typedef sib_disp_value<typename DispT::value_type> value_type;
			
				mod_rm_sib_disp( byte_t mod, byte_t rm, value_type const& disp_value )
					: mod_rm( mod, rm )
					, sib_disp_( disp_value )
				{}

				value_type sib_disp_;
			};
			typedef mod_rm_sib_disp<disp8_value>	mod_rm_sib_disp8;
			typedef mod_rm_sib_disp<disp32_value>	mod_rm_sib_disp32;


			typedef boost::variant<
				mod_rm,			// only mod_rm field
				mod_rm_disp8,	// mod_rm & disp(1Byte) field
				mod_rm_disp32,	// mod_rm & disp(4Byte) field
				mod_rm_sib,
				mod_rm_sib_disp8,
				mod_rm_sib_disp32
			> addressing;


			/*
			*/
			typedef detail::value_wrapper<addressing, byte_t>	r_m8;
			typedef detail::value_wrapper<addressing, word_t>	r_m16;
			typedef detail::value_wrapper<addressing, dword_t>	r_m32;

			struct r_m32_implicit
				: public detail::value_wrapper<addressing, dword_t>
			{
				r_m32_implicit( r_m32 value )		// implicit
					: value_wrapper( value ) {}

				r_m32_implicit( addressing value )	// implicit
					: value_wrapper( value ) {}

				r_m32_implicit( registers::detail::id<32> reg ) // implicit
					: value_wrapper( mod_rm( reg ) ) {}
			};


			namespace detail
			{
				inline byte_t make_mod_rm( byte_t const mod, byte_t const op_code, byte_t const rm )
				{
					return ( mod << 6 & 0xc0 ) | ( op_code << 3 & 0x38 ) | ( rm & 0x07 );
				}

				inline byte_t make_mod_rm( byte_t const op_code, byte_t const rm )
				{
					return make_mod_rm( 3, op_code, rm );
				}

				inline byte_t make_mod_rm( mod_rm const& mod_rm_value, registers::detail::id<32> const& reg )
				{
					return make_mod_rm( mod_rm_value.mod_, reg, mod_rm_value.rm_ );
				}

				inline byte_t make_sib( byte_t const ss, registers::detail::id<32> const index, registers::detail::id<32> const& base )
				{
					return ( ss << 6 & 0xc0 ) | ( index << 3 & 0x38 ) | ( base & 0x07 );
				}
			} // namespace detail

		} // namespace ia_32
	} // namespace assembler
} // namespace ytd

#endif /*YTL_ASSEMBLER_IA32_ADDRESS_HPP*/