#ifndef YTL_FILEFORMAT_COMMON_IMAGE_HPP
#define YTL_FILEFORMAT_COMMON_IMAGE_HPP

#include "../../../config.hpp"

namespace ytl
{
    namespace fileformat
    {
        namespace common
        {
            namespace image
            {
                // GUID - guiddef.h
#if defined( __midl )
                struct guid {
                    unsigned long  Data1;
                    unsigned short Data2;
                    unsigned short Data3;
                    byte_t         Data4[ 8 ];
                };
#else
                struct guid {
                    unsigned long  Data1;
                    unsigned short Data2;
                    unsigned short Data3;
                    unsigned char  Data4[ 8 ];
                };
#endif

                typedef guid        clsid;

            } // namespace image
        } // namespace common
    } // namespace fileformat
} // ytl

#endif /*YTL_FILEFORMAT_COMMON_IMAGE_HPP*/