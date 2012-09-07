#ifndef YTL_UTILITY_DUMP_HPP
#define YTL_UTILITY_DUMP_HPP

#include <cstdio>
#include <cstddef>

#include <ytl/config.hpp>

#include "has_wrapped_container_type.hpp"

namespace ytl
{
	namespace detail
	{
		template<template Buffer>
		void dumpbin_impl( Buffer const& bin, std::size_t const offset, std::size_t size, typename std::enable_if<!has_wrapped_container_type::value>::type* =0 )
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

		template<typename Buffer>
		inline void dumpbin_impl( Buffer const& bin, std::size_t const offset, std::size_t size, typename std::enable_if<has_wrapped_container_type::value>::type* =0 )
		{
			dumpbin_impl( (*bin), offset, size );
		}
	} // namespace detail

	template<typename Buffer>
	inline void dumpbin( Buffer const& bin, std::size_t const offset = 0, std::size_t size = 0 )
	{
		detail::dumpbin_impl( bin, offset, size );
	}

} // namespace ytl

#endif /*YTL_UTILITY_DUMP_HPP*/