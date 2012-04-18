#ifndef YTL_ASSEMBLER_IA32_GENERATOR_HPP
#define YTL_ASSEMBLER_IA32_GENERATOR_HPP

#include "../../config.hpp"
#include "../../writer.hpp"

#include "registers.hpp"
#include "data_types.hpp"
#include "aliginment.hpp"
#include "address.hpp"

#include <cassert>
#include <memory>

#include <iostream>

#define TYPICAL_OPERATOR(											\
			name,													\
			op8accimm8,												\
			op16_32accimm16_32,										\
			op_code,												\
			oprm8imm8,												\
			oprm16_32imm16_32,										\
			oprm16_32imm8,											\
			oprm8r8,												\
			oprm16_32r16_32,										\
			opr8rm8,												\
			opr16_32rm16_32											\
			)														\
	/* name AL, imm8 */												\
	void name( registers::detail::id<8> dst, dword_value src )		\
	{																\
		if ( dst == registers::al ) {								\
			db( op8accimm8 );										\
			db( src );												\
		} else {													\
			name( r_m8( mod_rm( dst ) ), byte_value( src ) );		\
		}															\
	}																\
	/* name AX, imm16 */											\
	void name( registers::detail::id<16> dst, dword_value src )		\
	{																\
		if ( dst == registers::ax ) {								\
			db( op16_32accimm16_32 );								\
			dw( src );												\
		} else {													\
			name( r_m16( mod_rm( dst ) ), byte_value( src ) );		\
		}															\
	}																\
	/* name EAX, imm32 */											\
	void name( registers::detail::id<32> dst, dword_value src )		\
	{																\
		if ( dst == registers::eax ) {								\
			db( op16_32accimm16_32 );								\
			dd( src );												\
		} else {													\
			name( r_m32( mod_rm( dst ) ), src );					\
		}															\
	}																\
	/* name r/m8, imm8 */											\
	void name( r_m8 dst, byte_value src )							\
	{																\
		db( oprm8imm8 );											\
		dmod_rm_sib_disp( dst, op_code );							\
		db( src );													\
	}																\
	/* name r/m16, imm16 */											\
	void name( r_m16 dst, word_value src )							\
	{																\
		db( oprm16_32imm16_32 );									\
		dmod_rm_sib_disp( dst, op_code );							\
		dw( src );													\
	}																\
	/* name r/m32, imm32 */											\
	void name( r_m32 dst, dword_value src )							\
	{																\
		db( oprm16_32imm16_32 );									\
		dmod_rm_sib_disp( dst, op_code );							\
		dd( src );													\
	}																\
	/* name r/m16, imm8 */											\
	void name( r_m16 dst, byte_value src )							\
	{																\
		db( oprm16_32imm8 );										\
		dmod_rm_sib_disp( dst, op_code );							\
		db( src );													\
	}																\
	/* name r/m32, imm8 */											\
	void name( r_m32 dst, byte_value src )							\
	{																\
		db( oprm16_32imm8 );										\
		dmod_rm_sib_disp( dst, op_code );							\
		db( src );													\
	}																\
	/* name r/m8, r8 */												\
	void name( r_m8 dst, registers::detail::id<8> src )				\
	{																\
		db( oprm8r8 );												\
		dmod_rm_sib_disp( dst, src );								\
	}																\
	/* name r/m16, r16 */											\
	void name( r_m16 dst, registers::detail::id<16> src )			\
	{																\
		db( oprm16_32r16_32 );										\
		dmod_rm_sib_disp( dst, src );								\
	}																\
	/* name r/m32, r32 */											\
	void name( r_m32_implicit dst, registers::detail::id<32> src )	\
	{																\
		db( oprm16_32r16_32 );										\
		dmod_rm_sib_disp( dst, src );								\
	}																\
	/* name r8, r/m8 */												\
	void name( registers::detail::id<8> dst, r_m8 src )				\
	{																\
		db( opr8rm8 );												\
		dmod_rm_sib_disp( src, dst );								\
	}																\
	/* name r16, r/m16 */											\
	void name( registers::detail::id<16> dst, r_m16 src )			\
	{																\
		db( opr16_32rm16_32 );										\
		dmod_rm_sib_disp( src ,dst );								\
	}																\
	/* name r32, r/m32 */											\
	void name( registers::detail::id<32> dst, r_m32 src )			\
	{																\
		db( opr16_32rm16_32 );										\
		dmod_rm_sib_disp( src, dst );								\
	}


namespace ytd
{
	namespace assembler
	{
		namespace ia_32
		{
			template<typename Buffer, typename Writter = assembler::detail::little_endian_writer>
			class generator
			{
				typedef generator	self_type;

				typedef Buffer		buffer_type;
				typedef Writter		writter_type;

			public:
				generator( buffer_type& buffer )
					: index_( 0u )
					, buffer_( buffer )
				{}

				buffer_type const& get_buffer() const
				{
					return buffer_;
				}

				//          --------------------------------------------
				void db( byte_t v )	{ write( v ); }
				void dw( word_t v )	{ write( v ); }
				void dd( dword_t v ){ write( v ); }

				// Mnemonic --------------------------------------------

				// -- A --

				// AAA
				void aaa() { db( 0x37 ); }


				// AAD
				void aad() { db( 0xd5 ); db( 0x0a ); }
				/*// AAD ib
				void aad( byte_value dst ) { db( 0xd5 ); db( dst ); }*/


				// AAM
				void aam() { db( 0xd4 ); db( 0x0a ); }
				/*// AAM ib
				void aam( byte_value dst ) { db( 0xD4 ); db( dst ); }*/


				// AAS
				void aas() { db( 0x3f ); }


				// ADC
				TYPICAL_OPERATOR( adc, 0x14, 0x15, 2, 0x80, 0x81, 0x83, 0x10, 0x11, 0x12, 0x13 );


				// ADD
				TYPICAL_OPERATOR( add, 0x04, 0x05, 0, 0x80, 0x81, 0x83, 0x00, 0x01, 0x02, 0x03 );


				// ADDPD

				// ADDPS

				// ADDSD

				// ADDSS

				// ADDSUBPD

				// ADDSUBPS


				// AND
				TYPICAL_OPERATOR( and, 0x24, 0x25, 4, 0x80, 0x81, 0x83, 0x20, 0x21, 0x22, 0x23 );


				// ANDPD

				// ANDPS

				// ANDNPD

				// ANDNPS


				// ARPL
				void arpl( r_m16 dst, registers::detail::id<16> src ) {
					db( 0x63 );
					dmod_rm_sib_disp( dst, src );
				}

				// -- B --
				// -- C --
				// -- D --
				// -- E --
				// -- F --
				// -- G --
				// -- H --
				// -- I --
				// -- J --
				// -- K --
				// -- L --
				// -- M --
				void mov( r_m8 dst, registers::detail::id<8> src )
				{}

				void mov( r_m16 dst, registers::detail::id<16> src )
				{}

				void mov( r_m32_implicit dst, registers::detail::id<32> src )
				{
					db( 0x89 );
					dmod_rm_sib_disp( dst, src );
				}

				void mov( registers::detail::id<8> dst, r_m8 src )
				{}

				void mov( registers::detail::id<16> dst, r_m16 src )
				{}

				void mov( registers::detail::id<32> dst, r_m32 src )
				{
					db( 0x8B );
					dmod_rm_sib_disp( src, dst );
				}

				void mov( registers::detail::id<8> dst, byte_value src )
				{
					db( 0xb0 + dst );
					dd( src );
				}
				void mov( registers::detail::id<16> dst, word_value src )
				{
					db( 0xb8 + dst );
					dd( src );
				}
				void mov( registers::detail::id<32> dst, dword_value src )
				{
					db( 0xb8 + dst );
					dd( src );
				}

				// -- N --
				void not( registers::detail::id<32> reg ) {
				}

				// -- O --

				//
				// -- P --
				//

				//
				void pop( registers::detail::id<32> reg )
				{
					db( 0x58 + reg );
				}

				// PUSH
				void push( registers::detail::id<16> reg )
				{
					db( 0x50 + reg );
				}

				void push( registers::detail::id<32> reg )
				{
					db( 0x50 + reg );
				}

				void push( byte_value value )
				{
					db( 0x6a );
					db( value );
				}

				void push( word_value value )
				{
					db( 0x68 );
					dw( value );
				}

				void push( dword_value value )
				{
					db( 0x68 );
					dw( value );
				}

				// -- Q --
				// -- R --
				void ret()
				{
					db( 0xc3 );
				}

				// -- S --

				// SUB
				TYPICAL_OPERATOR( sub, 0x2c, 0x2d, 5, 0x80, 0x81, 0x83, 0x28, 0x29, 0x2a, 0x2b );


				// -- T --
				// -- U --
				// -- V --
				// -- W --
				// -- X --
				// -- Y --
				// -- Z --



			private:
				template<typename T>
				void write( T const& value )
				{
					typename writter_type::write( &buffer_[0] + index_, std::addressof( value ) );
					index_ += sizeof( value );
				}

				template<typename T>
				class mod_rm_sib_disp_visitor
					: public boost::static_visitor<void>
				{
				public:
					mod_rm_sib_disp_visitor( byte_t reg_op, T* generator_ptr )
						: reg_op_( reg_op )
						, generator_ptr_( generator_ptr )
					{}

					void operator()( mod_rm const& v ) const
					{
						generator_ptr_->db( detail::make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
					}

					void operator()( mod_rm_disp8 const& v ) const
					{
						generator_ptr_->db( detail::make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						generator_ptr_->db( v.disp_ );
					}

					void operator()( mod_rm_disp32 const& v ) const
					{
						generator_ptr_->db( detail::make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						generator_ptr_->dd( v.disp_ );
					}

					void operator()( mod_rm_sib const& v ) const
					{
						generator_ptr_->db( detail::make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						generator_ptr_->db( detail::make_sib( v.sib_.ss_, v.sib_.index_, v.sib_.base_ ) );
					}

					void operator()( mod_rm_sib_disp8 const& v ) const
					{
						generator_ptr_->db( detail::make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						generator_ptr_->db( detail::make_sib( v.sib_disp_.sib_.ss_, v.sib_disp_.sib_.index_, v.sib_disp_.sib_.base_ ) );
						generator_ptr_->db( v.sib_disp_.disp_ );
					}

					void operator()( mod_rm_sib_disp32 const& v ) const
					{
						generator_ptr_->db( detail::make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						generator_ptr_->db( detail::make_sib( v.sib_disp_.sib_.ss_, v.sib_disp_.sib_.index_, v.sib_disp_.sib_.base_ ) );
						generator_ptr_->dd( v.sib_disp_.disp_ );
					}

				private:
					byte_t reg_op_;
					T* generator_ptr_;
				};
				template<typename T, unsigned int N>
				void dmod_rm_sib_disp( T const& data, registers::detail::id<N> reg )
				{
					boost::apply_visitor( mod_rm_sib_disp_visitor<self_type>( reg, this ), data.value_ );
				}
				template<typename T>
				void dmod_rm_sib_disp( T const& data, byte_t op_code )
				{
					boost::apply_visitor( mod_rm_sib_disp_visitor<self_type>( op_code, this ), data.value_ );
				}

				std::size_t index_;
				buffer_type& buffer_;
			};

		} // namespece ia_32
	} // namespace assembler
} // namespace ytd

#undef TYPICAL_OPERATOR

#endif /*YTL_ASSEMBLER_IA32_GENERATOR_HPP*/
