#ifndef YTL_UTILITY_DUMP_HPP
#define YTL_UTILITY_DUMP_HPP

#include <cstdio>
#include <cstddef>

#include <string>

#include <boost/lexical_cast.hpp>

#include <ytl/config.hpp>

#include "has_wrapped_container_type.hpp"

namespace ytl
{
	namespace detail
	{
		template<std::size_t N, typename Buffer>
		void dumpbin_impl(
			Buffer const& bin,
			std::size_t const offset,
			std::size_t size, typename std::enable_if<!has_wrapped_container_type<Buffer>::value>::type* =0
			)
		{
			static std::size_t const width = 16;

			if ( offset >= bin.size() ) {
				return;
			}

			if ( size < 0 ) {
				size = bin.size();
			} else if ( size > bin.size() + offset ) {
				size = bin.size() + offset;
			}

			std::string const space( N + 3, ' ' ), left_format( "%0" + boost::lexical_cast<std::string>( N ) + "X | " );

			std::printf( space.c_str() );
			for( unsigned i=0; i<width; ++i ) {
				std::printf( "%02X ", i );
			}
			std::printf( "\n" );

			std::printf( space.c_str() );
			for( std::size_t i=0; i<width; ++i ) {
				std::printf( "-- " );
			}
			std::printf( "  " );
			for( std::size_t i=0; i<width/2; ++i ) {
				std::printf( "-+" );
			}
			std::printf( "\n" );

			auto it = bin.cbegin() + offset;
			std::size_t line = 0;
			do {
				// address
				std::printf( left_format.c_str(), offset + line * width );

				// binary
				[&, it]() mutable {
					for( auto i=0; i<width; ++i ) {
						if ( it < bin.cend() ) {
							std::printf( "%02X ", static_cast<unsigned char>( *it ) );
							++it;
						} else {
							std::printf( "   " );
						}
					}
				}();
				std::printf( "| " );

				// ascii
				[&, it]() mutable {
					for( auto i=0; i<width; ++i ) {
						if ( it < bin.cend() ) {
							std::printf( "%c", ( *it < 0x20 || *it > 0x7e ) ? '.' : *it );
							++it;
						} else {
							std::printf( " " );
						}
					}
				}();
				std::printf( "\n" );

				for( auto i=0; i<width; ++i ) {
					if ( it < bin.cend() ) {
						++it;
					}
				}
				++line;
			} while( line < ( size + size % width ) / width );
		}

		template<std::size_t N, typename Buffer>
		inline void dumpbin_impl(
			Buffer const& bin,
			std::size_t const offset,
			std::size_t size, typename std::enable_if<has_wrapped_container_type<Buffer>::value>::type* =0
			)
		{
			dumpbin_impl<N>( (*bin), offset, size );
		}
	} // namespace detail

	template<typename Buffer>
	inline void dumpbin32( Buffer const& bin, std::size_t const offset = 0, std::size_t size = -1 )
	{
		detail::dumpbin_impl<8>( bin, offset, size );
	}

	template<typename Buffer>
	inline void dumpbin64( Buffer const& bin, std::size_t const offset = 0, std::size_t size = -1 )
	{
		detail::dumpbin_impl<16>( bin, offset, size );
	}

} // namespace ytl

#endif /*YTL_UTILITY_DUMP_HPP*/