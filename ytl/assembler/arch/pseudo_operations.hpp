#ifndef YTL_ASSEMBLER_PSEUDO_OPERATIONS_HPP
#define YTL_ASSEMBLER_PSEUDO_OPERATIONS_HPP

#include <cstddef>
#include <string>
#include <utility>

#include "../config.hpp"
#include "../exception.hpp"

#include "utility.hpp"

namespace ytl
{
	namespace assembler
	{
		template<template<typename> class Engine, typename Writer>
		class pseudo_operations
		{
			typedef Writer			writer_type;
			typedef Engine<Writer>	derived_type;

			typedef derived_type	return_type;

		public:
			pseudo_operations( writer_type& w )
				: writer_( w )
			{}

			//          --------------------------------------------
			YTL_ASM_OP_LAZY_1( db )
			YTL_ASM_OP_BEGIN( db( byte_t v ) )	writer_.write( v ); YTL_ASM_OP_END
			YTL_ASM_OP_LAZY_1( bw )
			YTL_ASM_OP_BEGIN( dw( word_t v ) )	writer_.write( v ); YTL_ASM_OP_END
			YTL_ASM_OP_LAZY_1( dd )
			YTL_ASM_OP_BEGIN( dd( dword_t v ) )	writer_.write( v ); YTL_ASM_OP_END

			//
			template<typename T>
			YTL_ASM_OP_BEGIN( embed( T const& v ) )
				writer_.write( v );
			YTL_ASM_OP_END

			//
			template<typename CharT>
			YTL_ASM_OP_BEGIN( msg( std::basic_string<CharT> const& str ) )
				writer_.write( str.data(), str.size() * sizeof(CharT) );
			YTL_ASM_OP_END

			template<typename CharT, std::size_t N>
			YTL_ASM_OP_BEGIN( msg( CharT const (&str)[N] ) )
				writer_.write( str, N * sizeof(CharT) );
			YTL_ASM_OP_END

			//
			YTL_ASM_OP_BEGIN( label( std::string const& l ) )
				// unimplemented
			YTL_ASM_OP_END

			//
			YTL_ASM_OP_LAZY_1( resb )
			YTL_ASM_OP_BEGIN( resb( std::size_t const count ) )
				for( std::size_t i=0; i<count; ++i )
					db( 0x00 );
			YTL_ASM_OP_END
			

		private:
			writer_type& writer_;
		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_PSEUDO_OPERATIONS_HPP*/