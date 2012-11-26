#ifndef YTL_FILEFORMAT_PE_ACCESSOR_HPP
#define YTL_FILEFORMAT_PE_ACCESSOR_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include <boost/optional.hpp>

//#include "image.hpp"
#include "../coff.hpp"

namespace ytl
{
    namespace fileformat
    {
        namespace pe
        {
            namespace op
            {
                struct validator
                {
                    template<typename Buffer>
                    void operator()( Buffer const& buffer ) const
                    {
                        if ( buffer.size() < sizeof( image::object_header ) ) {
                            throw std::domain_error( "Invalid anon format." );
                        }

                        image::object_header const* const header
                            = reinterpret_cast<image::object_header const*>( buffer.data() );

                        if ( header->sig1 != image::file_machine_unknown ) {
                            throw std::domain_error( "Invalid anon format." );
                        }

                        if ( header->sig2 != 0xffff ) {
                            throw std::domain_error( "Invalid anon format." );
                        }
                    }
                };
            } // namespace op


            template<typename Buffer>
            class accessor
            {
            public:
                typedef op::validator                                           validator_type;

            public:
                typedef Buffer                                                  buffer_type;

            public:
                accessor()
                    : dos_section_()
                {}

                
                void set_dos_section( buffer_type dos_section )
                {
                    dos_section_ = std::move( dos_section );
                }

            private:
                buffer_type dos_section_;
            };

        } // namespace pe
    } // namespace fileformat
} // namespace ytl
#endif /*YTL_FILEFORMAT_PE_ACCESSOR_HPP*/