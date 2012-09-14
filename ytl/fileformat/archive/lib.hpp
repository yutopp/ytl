#ifndef YTL_FILEFORMAT_ARCHIVE_LIB_HPP
#define YTL_FILEFORMAT_ARCHIVE_LIB_HPP

#include <cstddef>
#include <cstring>
#include <ctime>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <algorithm>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include "../config.hpp"

namespace ytl
{
	namespace fileformat
	{
		namespace archive
		{
			namespace image
			{
				std::size_t const start_size = 8;
				char const* const start = "!<arch>\n";
				char const* const end = "`\n";
				char const* const pad = "\n";
				char const* const linker_member = "/               ";
				char const* const longnames_member = "//              ";

				struct member_header
				{
					byte_t name[16];
					byte_t date[12];
					byte_t user_id[6];
					byte_t group_id[6];
					byte_t mode[8];
					byte_t size[10];
					byte_t end_header[2];
				};

				std::size_t const sizeof_member_hdr = 60;
			} // namespace image

			namespace lib_detail
			{
				template<typename ReturnT, typename T, std::size_t N>
				inline ReturnT binary_cast( T const (&bin)[N] )
				{
					return boost::lexical_cast<ReturnT>(
									boost::algorithm::replace_all_copy(
										std::string( reinterpret_cast<char const*>( bin ), reinterpret_cast<char const*>( bin ) + sizeof( bin ) )
										, " "
										, ""
										)
									);
				}


				struct managed_member_header
				{
					explicit managed_member_header( image::member_header const* p )
						: name( reinterpret_cast<char const*>( p->name ), reinterpret_cast<char const*>( p->name ) + sizeof( p->name ) )
						, date( binary_cast<std::time_t>( p->date ) )
						, size( binary_cast<std::size_t>( p->size ) )
					{
//						std::memcpy( user_id, p->user_id, sizeof( user_id ) );
//						std::memcpy( group_id, p->group_id, sizeof( group_id ) );
//						std::memcpy( mode, p->mode, sizeof( mode ) );
					}

				public:
					std::string name;
					std::time_t date;
//					byte_t user_id[6];
//					byte_t group_id[6];
//					byte_t mode[8];
					std::size_t size;
				};


				template<typename Buffer>
				struct archive
				{
					managed_member_header header;
					Buffer value;
				};


				class first_linker_member
				{
				public:
					explicit first_linker_member( image::member_header const* const raw_header )
						: header_( raw_header )
					{
						if ( std::memcmp( header_->name, image::linker_member, sizeof( header_->name ) ) != 0 ) {
							throw std::exception( "This is not a first linker member." );
						}

//						std::cout << raw_header->name << std::endl;
					}

				private:
					image::member_header const* header_;
				};


				class second_linker_member
				{
					typedef uint32_t		num_type;
					typedef uint32_t		offset_type;
					typedef uint16_t		index_type;
					typedef std::string		string_type;

				public:
					explicit second_linker_member( image::member_header const* const raw_header )
						: header_( raw_header )
						, buffer_( reinterpret_cast<byte_t const*>( raw_header ) + sizeof( image::member_header ) )
					{
						//
						if ( std::memcmp( header_->name, image::linker_member, sizeof( header_->name ) ) != 0 ) {
							throw std::exception( "This is not a second linker member." );
						}

						//
						num_type const member_num = *reinterpret_cast<num_type const*>( buffer_ );
//						std::cout << "member : " << member_num << std::endl;
						{
							auto pointer_offset = reinterpret_cast<offset_type const*>( buffer_ + sizeof( num_type ) );
							for( num_type i=0; i<member_num; ++i ) {
								offset_type const offset = *( pointer_offset + i );
								offsets_.push_back( offset );

//								std::cout << "offset : " << offset << std::endl;
							}
						}
						std::size_t offsets_offset = sizeof( num_type ) + member_num * sizeof( offset_type );

						//
						num_type const symbol_num = *reinterpret_cast<offset_type const*>( buffer_ + offsets_offset );
						std::cout << "symbol_num : " << symbol_num << std::endl;
						{
							auto pointer_offset = reinterpret_cast<index_type const*>( buffer_ + offsets_offset + sizeof( num_type ) );
							for( num_type i=0; i<symbol_num; ++i ) {
								index_type const index = *( pointer_offset + i );
								indices_.push_back( index - 1 );	// index is 1-based...

//								std::cout << "symbol_index : " << index << std::endl;
							}
						}
						std::size_t symbols_offset = offsets_offset + sizeof( num_type ) + symbol_num * sizeof( index_type );

						//
						{
							char const* it = reinterpret_cast<char const*>( buffer_ + symbols_offset );
							for( num_type i=0; i<symbol_num; ++i ) {
								std::string const name( it );
								it += name.size() + 1;
								symbols_.push_back( name );	

//								std::cout << "name : " << name << std::endl;
							}
						}
					}

					boost::optional<offset_type> get_offset_by_name( string_type const& symbol ) const
					{
						auto const it = std::lower_bound( symbols_.cbegin(), symbols_.cend(), symbol );
						if ( it == symbols_.cend() ) {
							return boost::none;
						}

						auto const index = std::distance( symbols_.cbegin(), it );
						return offsets_.at( indices_.at( index ) );
					}

				private:
					image::member_header const* header_;
					byte_t const* buffer_;
					
					std::vector<offset_type> offsets_;
					std::vector<index_type> indices_;
					std::vector<string_type> symbols_;
				};


				class longnames
				{
					typedef uint32_t		offset_type;
					typedef std::string		string_type;

				public:
					explicit longnames( image::member_header const* const raw_header )
						: header_( raw_header )
						, buffer_( reinterpret_cast<byte_t const*>( raw_header ) + sizeof( image::member_header ) )
					{
						if ( std::memcmp( raw_header->name, image::longnames_member, sizeof( raw_header->name ) ) != 0 ) {
							//throw std::exception( "This is not a longnames member." );
							return;		// this header may be optional.
						}

//						std::cout << raw_header->name << std::endl;
					}

					std::string get_name_by_offset( offset_type const offset ) const
					{
						if ( offset >= header_.size ) {
							throw std::exception( "Out of range." );
						}

						std::string const name( reinterpret_cast<char const*>( buffer_ ) + offset );
						names_[offset] = name;

						return name;
					}

				private:
					managed_member_header header_;
					byte_t const* buffer_;

					mutable std::map<offset_type, string_type> names_;
				};
			} // namespace lib_detail
			

			template<template <typename T> class Allocator>
			class basic_lib
			{
				typedef ytl::assembler::basic_binary<Allocator>		binary_type;
				typedef std::shared_ptr<binary_type>				binary_pointer_type;

			private:
				class binary_wrapper
				{
				public:
					binary_wrapper( binary_pointer_type const& p )
						: p_( p )
					{
						validate();
					}

					binary_type& operator*() { return *p_; }
					binary_type const& operator*() const { return *p_; }
					binary_type& operator->() { return *(*this); }
					binary_type const& operator->() const { return *(*this); }

				private:
					void validate() const
					{
						if ( (*p_)->size() < image::start_size ) {
							throw std::exception( "Invalid lib format." );
						}

						if ( std::memcmp( (*p_)->data(), image::start, image::start_size ) != 0 ) {
							throw std::exception( "Invalid header." );
						}
					}

				private:
					binary_pointer_type p_;
				};
			public:
				typedef	lib_detail::archive<binary_type>			archive_type;
				typedef std::shared_ptr<archive_type const>			archive_pointer_type;
				typedef std::size_t									offset_type;
				typedef std::size_t									index_type;

			public:
				basic_lib( binary_pointer_type const& p )
					: raw_( p )
					, first_( get_archive_header_pointer_by_index( 0 ) )
					, second_( get_archive_header_pointer_by_index( 1 ) )
					, longnames_( get_archive_header_pointer_by_index( 2 ) )
				{}

				template<typename Buffer>
				basic_lib( Buffer const& bin )
					: raw_( std::make_shared<binary_type>( bin ) )
					, first_( get_archive_header_pointer_by_index( 0 ) )
					, second_( get_archive_header_pointer_by_index( 1 ) )
					, longnames_( get_archive_header_pointer_by_index( 2 ) )
				{}

				template<typename IterT>
				basic_lib( IterT const& begin, IterT const& end )
					: raw_( std::make_shared<binary_type>( begin, end ) )
					, first_( get_archive_header_pointer_by_index( 0 ) )
					, second_( get_archive_header_pointer_by_index( 1 ) )
					, longnames_( get_archive_header_pointer_by_index( 2 ) )
				{}

			public:
				archive_pointer_type find_archive_by_symbol( std::string const& symbol ) const
				{
					auto const offset = second_.get_offset_by_name( symbol );
					if ( !offset ) {
						return archive_pointer_type( nullptr );
					}

					if ( memo_.find( *offset ) == memo_.end() ) {
						memo_[ *offset ] = get_archive_by_offset( *offset );
					}

					return memo_.at( *offset );
				}

			private:
				image::member_header const* get_archive_header_pointer_by_offset( offset_type const offset ) const
				{
					if ( offset >= raw_->size() ) {
						throw std::exception( "Out of range." );
					}

					image::member_header const* const header =
						reinterpret_cast<image::member_header const* const>( raw_->data() + offset );

					if ( memcmp( header->end_header, image::end, sizeof( header->end_header ) ) != 0 ) {
						throw std::exception( "Invalid archive end header." );
					}

					return header;
				}

				std::size_t get_archive_size_by_offset( offset_type const offset ) const
				{
					image::member_header const* const header = get_archive_header_pointer_by_offset( offset );

					return lib_detail::binary_cast<std::size_t>( header->size );
				}

				archive_pointer_type get_archive_by_offset( offset_type const offset ) const
				{
					image::member_header const* const header = get_archive_header_pointer_by_offset( offset );

					auto const begin = raw_->data() + offset + sizeof( image::member_header );
					std::size_t const size = get_archive_size_by_offset( offset );
					archive_type h = {
						lib_detail::managed_member_header( header ),
						binary_type( begin, begin + size )
					};

					return std::make_shared<archive_type>( h );
				}

				offset_type get_archive_offset_by_index( index_type const index ) const
				{
					offset_type offset = image::start_size;
					
					for( std::size_t i=0; i<index; ++i ) {
						auto size = get_archive_size_by_offset( offset );

						offset += sizeof( image::member_header ) + size;
						
						// lib file format is 2Byte aliginment.
						if ( offset % 2 != 0 ) {
							++offset;
						}
					}

					return offset;
				}

				image::member_header const* get_archive_header_pointer_by_index( index_type const index ) const
				{
					return get_archive_header_pointer_by_offset( get_archive_offset_by_index( index ) );
				}

			private:
				binary_wrapper raw_;

				lib_detail::first_linker_member first_;
				lib_detail::second_linker_member second_;
				lib_detail::longnames longnames_;

				mutable std::unordered_map<offset_type, archive_pointer_type> memo_;
			};

			//
			typedef basic_lib<std::allocator> lib;


			// read a binary from a file and build a object
			template<typename Dist, typename CharT, template <typename T> class Allocator>
			Dist build_object( CharT const* const filename, basic_lib<Allocator>* )
			{
				auto p = std::make_shared<ytl::assembler::basic_binary<Allocator>>();
				ytl::read_binary( filename, *p );

				return Dist( p );
			}

		} // namespace archive
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_ARCHIVE_LIB_HPP*/