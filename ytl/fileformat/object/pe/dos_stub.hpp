#ifndef YTL_FILEFORMAT_PE_EXEC_DOSSTUB_HPP
#define YTL_FILEFORMAT_PE_EXEC_DOSSTUB_HPP

#include <cstddef>

#include <stdexcept>

#include <ytl/assembler.hpp>

#include "../../config.hpp"

namespace ytl
{
    namespace fileformat
    {
        namespace pe
        {
            namespace dos
            {
                namespace image
                {
                    struct header
                    {
                        word_t  e_magic;
                        word_t  e_cblp;
                        word_t  e_cp;
                        word_t  e_crlc;
                        word_t  e_cparhdr;
                        word_t  e_minalloc;
                        word_t  e_maxalloc;
                        word_t  e_ss;
                        word_t  e_sp;
                        word_t  e_csum;
                        word_t  e_ip;
                        word_t  e_cs;
                        word_t  e_lfarlc;
                        word_t  e_ovno;
                        word_t  e_res[4];
                        word_t  e_oemid;
                        word_t  e_oeminfo;
                        word_t  e_res2[10];
                        dword_t e_lfanew;
                    };
                } // namespace image

                class default_code_genarator
                {
                public:
                    ytl::assembler::binary_code operator()() const
                    {
                        namespace x86 = ytl::assembler::x86;
                        namespace reg = x86::registers;

                        ytl::assembler::generator<x86::engine> as;
                        ytl::assembler::binary_code bin;
                        
                        // create DOS stub code
                        as( bin )
                            .push   ( reg::cs )
                            .pop    ( reg::ds )
                            .mov    ( reg::dx, 0x0e )
                            .mov    ( reg::ah, 0x09 )
                            .int_   ( 0x21 )
                            .mov    ( reg::ax, 0x4c01 )
                            .int_   ( 0x21 )
                            .label  ( "dos_msg" )
                            .msg    ( "This program cannot be run in DOS mode.\r\r\n$" );

                        return bin;
                    }
                };

                class default_header_genarator
                {
                public:
                    image::header operator()( dword_t const pe_start ) const
                    {
                        // create DOS header
                        image::header dos_h = {};
                        dos_h.e_magic       = 0x5a4d;       // Signature(MZ)
                        dos_h.e_cblp        = 0x90;         //
                        dos_h.e_cp          = 3;            //
                        dos_h.e_cparhdr     = 4;            //
                        dos_h.e_maxalloc    = 0xffff;       //
                        dos_h.e_sp          = 0xb8;         // initial value of stack pointer
                        dos_h.e_lfarlc      = 0x40;         //
                        dos_h.e_lfanew      = pe_start;

                        return dos_h;
                    }
                };

                template<
                    typename HeaderGen = dos::default_header_genarator,
                    typename CodeGen = dos::default_code_genarator
                >
                class dos_section_genarator
                {
                public:
                    typedef HeaderGen       header_gen_type;
                    typedef CodeGen         code_gen_type;

                public:
                    dos_section_genarator( unsigned int const pe_start = 0xe0 )
                        : pe_start_( pe_start )
                    {
                        if ( pe_start_ <= raw_size() )
                            throw std::runtime_error( "dos_section_genarator :: pe_start_ < raw_size()" );
                    }

                    static std::size_t raw_size()
                    {
                        return code_gen_type()()->size();
                    }

                    ytl::binary_buffer raw_buffer() const
                    {
                        ytl::assembler::binary_code code = (*this)();

                        return ytl::binary_buffer( std::move( code.get() ) );
                    }

                    ytl::assembler::binary_code operator()() const
                    {
                        namespace x86 = ytl::assembler::x86;

                        ytl::assembler::generator<x86::engine> as;
                        auto const& body = code_gen_type()();

                        ytl::assembler::binary_code binary;

                        as( binary )
                            .embed      ( header_gen_type()( pe_start_ ) )      // generate dos header
                            .raw_embed  ( body->data(), body->size() )          // generate dos stub
                            .resb       ( pe_start_ - as.$ );

                        return binary;
                    }

                private:
                    unsigned int pe_start_;
                };

            } // namespace dos
        } // namespace pe
    } // namespace fileformat
} // namespace ytl


#endif /*YTL_FILEFORMAT_PE_EXEC_DOSSTUB_HPP*/