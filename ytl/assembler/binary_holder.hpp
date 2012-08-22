#ifndef YTL_ASSEMBLER_BINARYHOLDER_HPP
#define YTL_ASSEMBLER_BINARYHOLDER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <iterator>
#include <algorithm>

#include <cstdio>
#include <cstddef>

#include "config.hpp"
#include "arch/detail/asm_status.hpp"

namespace ytl
{
	namespace assembler
	{
		template<template <typename T> class Allocator = std::allocator>
		class binary_holder
		{
			typedef binary_holder							self_type;

		public:
			typedef byte_t									byte_type;
			typedef Allocator<byte_type>					allocator_type;
			typedef std::vector<byte_type, allocator_type>	container_type;
			typedef typename container_type::size_type		size_type;

			typedef detail::asm_status						asm_status_type;
			typedef std::shared_ptr<asm_status_type>		asm_status_pointer_type;
			typedef std::shared_ptr<asm_status_type const>	asm_status_const_pointer_type;

		public:
			binary_holder()
				: status_( std::make_shared<asm_status_type>() )
			{}
			
			explicit binary_holder( std::string const& filename )
				: status_( std::make_shared<asm_status_type>() )
			{
				load_file( filename );
			}

			template<typename IterT>
			binary_holder( IterT const& begin, IterT const& end )
				: binary_( begin, end )
				, status_( std::make_shared<asm_status_type>() )
			{}

			container_type& get() { return binary_; }
			container_type const& get() const { return binary_; }

			container_type& operator*() { return get(); }
			container_type const& operator*() const { return get(); }

			container_type* operator->() { return &get(); }
			container_type const* operator->() const { return &get(); }

			asm_status_pointer_type get_status() { return status_; }
			asm_status_const_pointer_type get_status() const { return status_; }

		private:
			template<typename CharT>
			void load_file( std::basic_string<CharT> const& filename )
			{
				std::basic_ifstream<byte_type> ifs( filename, std::ios::binary );
				if ( !ifs )
					throw std::exception( "" );;

				std::istreambuf_iterator<byte_type> const begin( ifs ), end;
				std::copy( begin, end, std::back_inserter( binary_ ) );
			}

		private:
			container_type binary_;
			asm_status_pointer_type status_;
		};


		template<template <typename> class T, template <typename> class U>
		inline binary_holder<T>& operator<<( binary_holder<T>& lhs, binary_holder<U> const& rhs )
		{
			lhs->insert( lhs->end(), rhs->cbegin(), rhs->cend() );
			return lhs;
		}

		template<template <typename> class T, template <typename> class U, std::size_t N>
		inline binary_holder<T>& operator<<( binary_holder<T>& lhs, typename binary_holder<U>::byte_type const (&rhs)[N] )
		{
			lhs->insert( lhs->end(), rhs, rhs + N );
			return lhs;
		}

		template<template <typename> class T>
		inline std::ostream& operator<<( std::ostream& os, binary_holder<T> const& buffer )
		{
			os.write( reinterpret_cast<char const*>( &buffer->data()[0] ), buffer->size() );
			return os;
		}

		template<template <typename> class T>
		inline void dumpbin( binary_holder<T> const& bin, std::size_t const offset = 0, std::size_t size = 0 )
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

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_BINARYHOLDER_HPP*/