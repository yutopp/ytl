#ifndef YTL_FILEFORMAT_ANON_CORE_HPP
#define YTL_FILEFORMAT_ANON_CORE_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include <boost/optional.hpp>

#include "image.hpp"
#include "../coff.hpp"

namespace ytl
{
    namespace fileformat
    {
        namespace anon
        {
            namespace detail
            {
//                using namespace ytl::fileformat::detail;

            } // namespace detail

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


                struct v1_validator
                {
                    template<typename Buffer>
                    void operator()( Buffer const& buffer ) const
                    {
                        validator()( buffer );

                        if ( header->version < 1 ) {
                            throw std::domain_error( "Invalid anon v1 format." );
                        }
                    }
                };


                struct v2_validator
                {
                    template<typename Buffer>
                    void operator()( Buffer const& buffer ) const
                    {
                        validator()( buffer );

                        if ( header->version < 2 ) {
                            throw std::domain_error( "Invalid anon v2 format." );
                        }
                    }
                };


            } // namespace op


        } // namespace anon
    } // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_ANON_CORE_HPP*/