#ifndef YTL_FILEFORMAT_UTILITY_HPP
#define YTL_FILEFORMAT_UTILITY_HPP

#include <cstdio>
#include <cstddef>

#include "binary_holder.hpp"

namespace ytl
{
	namespace fileformat
	{
		inline void dumpbin( binary_holder const& bin, std::size_t const offset = 0, std::size_t size = 0 )
		{
			if ( size == 0 ) {
				size = bin->size();
			}
			if ( offset >= bin->size() ) {
				return;
			}

			std::printf( "         " );
			for( std::size_t i=0; i<16; ++i ) {
				std::printf( "%02X ", i );
			}
			std::printf( "\n" );

			std::printf( "         " );
			for( std::size_t i=0; i<16; ++i ) {
				std::printf( "-- ", i );
			}
			std::printf( "\n" );

			auto it = bin->cbegin() + offset;
			for( std::size_t i=0; i<size/16; ++i ) {
				std::printf( "%06X | ", offset + i * 16 );
				{
					auto tempit = it;
					for( std::size_t j=0; j<16; ++j, ++tempit ) {
						if ( tempit != bin->cend() ) {
							std::printf( "%02X ", static_cast<unsigned char>( *tempit ) );
						} else {
							std::printf( "   " );
						}
					}
				}
				std::printf( "| " );

				for( std::size_t j=0; j<16; ++j, ++it ) {
					if ( it != bin->cend() ) {
						std::printf( "%c", ( *it < 0x20 || *it > 0x7e ) ? '.' : *it );
					} else {
						std::printf( " " );
					}
				}
				std::printf( "\n" );
			}
		}

	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_UTILITY_HPP*/