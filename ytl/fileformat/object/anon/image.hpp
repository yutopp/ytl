#ifndef YTL_FILEFORMAT_ANON_IMAGE_HPP
#define YTL_FILEFORMAT_ANON_IMAGE_HPP

#include "../../../config.hpp"
#include "../common/image.hpp"

namespace ytl
{
    namespace fileformat
    {
        namespace anon
        {
            namespace image
            {
                using namespace common::image;

                struct object_header {
                    word_t      sig1;               // Must be IMAGE_FILE_MACHINE_UNKNOWN
                    word_t      sig2;               // Must be 0xffff
                    word_t      version;            // >= 1 (implies the CLSID field is present)
                    word_t      machine;
                    dword_t     time_date_stamp;
                    clsid       class_id;           // Used to invoke CoCreateInstance
                    dword_t     size_of_data;       // Size of data that follows the header
                };

                typedef struct object_header_v2 {
                    word_t      sig1;               // Must be IMAGE_FILE_MACHINE_UNKNOWN
                    word_t      sig2;               // Must be 0xffff
                    word_t      version;            // >= 2 (implies the Flags field is present - otherwise V1)
                    word_t      machine;
                    dword_t     time_date_stamp;
                    clsid       class_id;           // Used to invoke CoCreateInstance
                    dword_t     size_of_data;       // Size of data that follows the header
                    dword_t     flags;              // 0x1 -> contains metadata
                    dword_t     meta_data_size;     // Size of CLR metadata
                    dword_t     meta_data_offset;   // Offset of CLR metadata
                };

                typedef struct object_header_bigobj {
                    // same as object_header_v2
                    word_t      sig1;               // Must be IMAGE_FILE_MACHINE_UNKNOWN
                    word_t      sig2;               // Must be 0xffff
                    word_t      version;            // >= 2 (implies the Flags field is present)
                    word_t      machine;            // Actual machine - IMAGE_FILE_MACHINE_xxx
                    dword_t     time_date_stamp;
                    clsid       class_id;           // {D1BAA1C7-BAEE-4ba9-AF20-FAF66AA4DCB8}
                    dword_t     size_of_data;       // Size of data that follows the header
                    dword_t     flags;              // 0x1 -> contains metadata
                    dword_t     meta_data_size;     // Size of CLR metadata
                    dword_t     meta_data_offset;   // Offset of CLR metadata

                    // bigobj specifics
                    dword_t     number_of_sections; // extended from WORD
                    dword_t     pointer_to_symbol_table;
                    dword_t     number_of_symbols;
                };

            } // namespace image
        } // namespace anon
    } // namespace fileformat
} // ytl

#endif /*YTL_FILEFORMAT_ANON_IMAGE_HPP*/