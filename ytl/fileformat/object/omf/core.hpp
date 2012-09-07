#ifndef YTL_FILEFORMAT_OMF_CORE_HPP
#define YTL_FILEFORMAT_OMF_CORE_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include <boost/optional.hpp>

#include <ytl/buffer/shared_binary_range.hpp>

#include "../../config.hpp"
#include "../../detail/valid_buffer_holder.hpp"
#include "../../detail/utils.hpp"

//#include "image.hpp"

namespace ytl
{
    namespace fileformat
    {
        namespace omf
        {
            namespace detail
            {
                using namespace ytl::fileformat::detail;

            } // namespace detail


            namespace op
            {
                struct validator
                {
                    template<typename T>
                    void operator()( T const& buffer ) const
                    {
//                        if ( buffer.size() < sizeof( image::file_header ) ) {
//                            throw std::runtime_error( "Invalid omf format." );
//                        }
                    }
                };
            } // namespace op




        } // namespace omf
    } // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_OMF_CORE_HPP*/