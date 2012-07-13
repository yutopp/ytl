#ifndef YTL_FILEFORMAT_PECOFF_EXEC_DOSSTUB_HPP
#define YTL_FILEFORMAT_PECOFF_EXEC_DOSSTUB_HPP

#include <cstddef>

#include <ytl/assembler.hpp>

#include "../../config.hpp"
#include "../../binary_holder.hpp"

namespace ytl
{
	namespace fileformat
	{
		namespace pecoff
		{
			class dos_header_genarator
			{
			public:
				dos_header_genarator( unsigned int const pe_start = 0xe0 )
					: pe_start_( pe_start )
				{
					if ( pe_start < raw_size() )
						throw std::exception();
				}

				static std::size_t raw_size()
				{
					binary_holder binary;
					generate_stub( binary, 0/*dummy*/ );

					return binary->size();
				}

				binary_holder operator()() const
				{
					namespace x86 = ytl::assembler::x86;

					binary_holder binary;
					generate_stub( binary, pe_start_ );

					auto as = ytl::assembler::make_generator<x86::engine>( binary.get() );
					as()
						.resb	( pe_start_ - as.$ );

					return binary;
				}

				void static generate_stub( binary_holder& bin, unsigned int const pe_start )
				{
					namespace x86 = ytl::assembler::x86;
					namespace reg = x86::registers;
					
					// create DOS header
					::IMAGE_DOS_HEADER dos_h = {};
					dos_h.e_magic		= 0x5a4d;	// シグネチャ				: offset 0byte
					dos_h.e_cblp		= 0x90;		//							: offset 2byte
					dos_h.e_cp			= 3;		//							: offset 4byte
					dos_h.e_cparhdr		= 4;		//							: offset 8byte
					dos_h.e_maxalloc	= 0xffff;	//							: offset 12byte
					dos_h.e_sp			= 0xb8;		// スタックポインタの初期値	: offset 16byte
					dos_h.e_lfarlc		= 0x40;		//							: offset 24byte
					dos_h.e_lfanew		= pe_start;

					// create DOS stub code
					auto as = ytl::assembler::make_generator<x86::engine>( bin.get() );
					as()
						.embed	( dos_h )
						.push	( reg::cs )
						.pop	( reg::ds )
						.mov	( reg::dx, 0x0e )
						.mov	( reg::ah, 0x09 )
						.int_	( 0x21 )
						.mov	( reg::ax, 0x4c01 )
						.int_	( 0x21 )
						.label	( "dos_msg" )
						.msg	( "This program cannot be run in DOS mode.\r\r\n$" );
				}

			private:
				unsigned int pe_start_;
			};
		}
	}
};


#endif /*YTL_FILEFORMAT_PECOFF_EXEC_DOSSTUB_HPP*/