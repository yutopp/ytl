#ifndef YTL_ASSEMBLER_PSEUDO_OPERATIONS_HPP
#define YTL_ASSEMBLER_PSEUDO_OPERATIONS_HPP

#include <cstddef>
#include <string>
#include <memory>
#include <type_traits>
#include <utility>

#include "../../config.hpp"
#include "../../exception.hpp"
#include "../utility.hpp"

#include "asm_status.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace detail
		{
			template<typename Derived, typename Writer>
			class pseudo_operations
			{			
				typedef Derived							derived_type;

			public:
				typedef Writer							writer_type;
				typedef std::shared_ptr<writer_type>	writer_pointer_type;
				typedef detail::asm_status				status_type;
				typedef std::shared_ptr<status_type>	status_pointer_type;

				typedef std::size_t						size_type;
				typedef derived_type					return_type;

			public:
				pseudo_operations( writer_pointer_type w, status_pointer_type s )
					: writer_( w )
					, status_( s )
				{}

				//          --------------------------------------------
				YTL_ASM_OP_LAZY_1( db )
				inline YTL_ASM_OP_BEGIN( db( byte_t v ) )
					embed( v );
				YTL_ASM_OP_END

				YTL_ASM_OP_LAZY_1( bw )
				inline YTL_ASM_OP_BEGIN( dw( word_t v ) )
					embed( v );
				YTL_ASM_OP_END

				YTL_ASM_OP_LAZY_1( dd )
				inline YTL_ASM_OP_BEGIN( dd( dword_t v ) )
					embed( v );
				YTL_ASM_OP_END


				//
				template<typename T>
				inline YTL_ASM_OP_BEGIN( raw_embed( T const* const p, std::size_t const size ) )
					writer_->write( p, size );
				YTL_ASM_OP_END

				//
				template<typename T>
				inline YTL_ASM_OP_BEGIN( embed( T const& v, std::size_t const size = sizeof(T) ) )
					raw_embed( std::addressof( v ), size );
				YTL_ASM_OP_END

				//
				template<typename CharT>
				YTL_ASM_OP_BEGIN( msg( std::basic_string<CharT> const& str ) )
					raw_embed( str.c_str(), str.size() * sizeof(CharT) );
				YTL_ASM_OP_END

				template<typename CharT, std::size_t N>
				YTL_ASM_OP_BEGIN( msg( CharT const (&str)[N] ) )
					embed( str, N * sizeof(CharT) );
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
				

				std::size_t size() const
				{
					return writer_->get_index_cref();
				}

				YTL_ASM_OP_BEGIN( size( std::size_t& r ) )
					r = size();
				YTL_ASM_OP_END

			private:
				writer_pointer_type writer_;
				status_pointer_type status_;
			};

		} // namespace detail
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_PSEUDO_OPERATIONS_HPP*/