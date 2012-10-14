#ifndef YTL_FILEFORMAT_AR_HPP
#define YTL_FILEFORMAT_AR_HPP

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
#include <stdexcept>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <ytl/buffer/shared_binary_range.hpp>
#include <ytl/utility/guard_macros.hpp>

#include "../config.hpp"
#include "../detail/valid_buffer_holder.hpp"
#include "../detail/utils.hpp"

namespace ytl
{
	namespace fileformat
	{
		namespace ar
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


			struct managed_member_header
			{
				explicit managed_member_header( image::member_header const* p )
					: name( reinterpret_cast<char const*>( p->name ), reinterpret_cast<char const*>( p->name ) + sizeof( p->name ) )
					, date( detail::binary_cast<std::time_t>( p->date ) )
					, size( detail::binary_cast<std::size_t>( p->size ) )
				{
//					std::memcpy( user_id, p->user_id, sizeof( user_id ) );
//					std::memcpy( group_id, p->group_id, sizeof( group_id ) );
//					std::memcpy( mode, p->mode, sizeof( mode ) );
				}

				template<typename F>
				managed_member_header( image::member_header const* p, F&& f )
					: name( f( reinterpret_cast<char const*>( p->name ), reinterpret_cast<char const*>( p->name ) + sizeof( p->name ) ) )
					, date( detail::binary_cast<std::time_t>( p->date ) )
					, size( detail::binary_cast<std::size_t>( p->size ) )
				{
//					std::memcpy( user_id, p->user_id, sizeof( user_id ) );
//					std::memcpy( group_id, p->group_id, sizeof( group_id ) );
//					std::memcpy( mode, p->mode, sizeof( mode ) );
				}

			public:
				std::string name;
				std::time_t date;
//				byte_t user_id[6];
//				byte_t group_id[6];
//				byte_t mode[8];
				std::size_t size;
			};


			template<typename Buffer>
			struct archive
			{
				managed_member_header header;
				Buffer body;
			};


			class first_linker_member
			{
			public:
				explicit first_linker_member( image::member_header const* const raw_header )
					: header_( raw_header )
				{
					if ( std::memcmp( header_->name, image::linker_member, sizeof( header_->name ) ) != 0 ) {
						throw std::runtime_error( "This is not a first linker member." );
					}

//					std::cout << raw_header->name << std::endl;
				}

			private:
				image::member_header const* header_;
			};


			class second_linker_member
			{
				typedef uint32_t					num_type;

			public:
				typedef uint32_t					offset_type;
				typedef uint16_t					index_type;
				typedef std::string					string_type;

				typedef std::vector<offset_type>	offsets_type;
				typedef std::vector<index_type>		indices_type;
				typedef std::vector<string_type>	symbols_type;

			public:
				explicit second_linker_member( image::member_header const* const raw_header )
					: header_( raw_header )
					, buffer_( reinterpret_cast<byte_t const*>( raw_header ) + sizeof( image::member_header ) )
				{
					// check sigunature
					if ( std::memcmp( header_->name, image::linker_member, sizeof( header_->name ) ) != 0 ) {
						throw std::runtime_error( "This is not a second linker member." );
					}

					//
					num_type const member_num = *reinterpret_cast<num_type const*>( buffer_ );
					std::cout << "member : " << member_num << std::endl;
					{
						auto pointer_offset = reinterpret_cast<offset_type const*>( buffer_ + sizeof( num_type ) );
						for( num_type i=0; i<member_num; ++i ) {
							offset_type const offset = *( pointer_offset + i );
							offsets_.push_back( offset );

//							std::cout << "offset : " << offset << std::endl;
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

//							std::cout << "symbol_index : " << index << std::endl;
						}
					}
					std::size_t symbols_offset = offsets_offset + sizeof( num_type ) + symbol_num * sizeof( index_type );

					//
					{
						char const* it = reinterpret_cast<char const*>( buffer_ + symbols_offset );
						for( num_type i=0; i<symbol_num; ++i ) {
							std::string name( it );
							it += name.size() + 1;

							symbols_.emplace_back( std::move( name ) );

							//std::cout << "name : " << name << std::endl;
						}
					}
				}


				boost::optional<offset_type> get_offset_by_symbol( string_type const& symbol ) const
				{
					auto const it = std::lower_bound( symbols_.cbegin(), symbols_.cend(), symbol );
					if ( it == symbols_.cend() ) {
						return boost::none;
					}
					auto const index = std::distance( symbols_.cbegin(), it );

					return get_offset_by_index( indices_.at( index ) );
				}

				boost::optional<offset_type> get_offset_by_index( index_type const index ) const
				{
					if ( index < 0 || index >= offsets_.size() ) {
						return boost::none;
					}

					return offsets_.at( index );
				}

				offsets_type const& get_offset_table() const
				{
					return offsets_;
				}

				indices_type const& get_index_table() const
				{
					return indices_;
				}

				symbols_type const& get_symbol_table() const
				{
					return symbols_;
				}

			private:
				image::member_header const* header_;
				byte_t const* buffer_;
				
				offsets_type offsets_;
				indices_type indices_;
				symbols_type symbols_;
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
					// check sigunature
					if ( std::memcmp( raw_header->name, image::longnames_member, sizeof( raw_header->name ) ) != 0 ) {
						//throw std::exception( "This is not a longnames member." );
						header_.size = -1;
						return;		// this header may be optional.
					}

//					std::cout << "Lib longnames_: " << raw_header->name << std::endl;
				}

				std::string get_name_by_offset( offset_type const offset ) const
				{
					if ( offset < 0 || offset >= header_.size ) {
						throw std::runtime_error( "Out of range." );
					}

					std::string const name( reinterpret_cast<char const*>( buffer_ ) + offset );
//					names_[offset] = name;

					return name;
				}

			private:
				managed_member_header header_;
				byte_t const* buffer_;

//				mutable std::map<offset_type, string_type> names_;
			};


			//template<typename Buffer>
			class immutable_accessor
			{
			private:
				struct validator
				{
					template<typename T>
					void operator()( T const& p ) const
					{
						if ( p->size() < image::start_size ) {
							throw std::runtime_error( "Invalid lib format." );
						}

						if ( std::memcmp( p->data(), image::start, image::start_size ) != 0 ) {
							throw std::runtime_error( "Invalid header." );
						}
					}
				};

			private:
				typedef detail::immutable_buffer_holder<validator>		holder_type;
				typedef std::shared_ptr<holder_type const>				holder_shared_pointer;

				typedef const_shared_binary_range<holder_type>			buffer_type;

			public:
				typedef	archive<buffer_type>							archive_type;
				typedef std::shared_ptr<archive_type>					archive_pointer_type;
				typedef std::size_t										offset_type, index_type;

			public:
				// move / copy / unique_ptr
				template<typename Buffer>
				immutable_accessor( Buffer&& buffer )
					: holder_ptr_( std::make_shared<holder_type>( std::forward<Buffer>( buffer ) ) )
					, first_( get_archive_header_pointer_by_index( 0 ) )
					, second_( get_archive_header_pointer_by_index( 1 ) )
					, longnames_( get_archive_header_pointer_by_index( 2 ) )	// may be existed
				{}

			public:
				archive_pointer_type get_archive_by_symbol( std::string const& symbol ) const
				{
					auto const offset = second_.get_offset_by_symbol( symbol );
					if ( !offset ) {
						return archive_pointer_type( nullptr );
					}

					return get_archive_by_offset_with_memoize( *offset );
				}

				archive_pointer_type get_archive_at( std::size_t index ) const
				{
					auto const offset = second_.get_offset_by_index( index );
					if ( !offset ) {
						return archive_pointer_type( nullptr );
					}

					return get_archive_by_offset_with_memoize( *offset );
				}

				// not include special member num
				std::size_t get_archives_num() const
				{
					return second_.get_offset_table().size();
				}

				second_linker_member::symbols_type get_symbol_table() const
				{
					return second_.get_symbol_table();
				}

			private:
				image::member_header const* get_archive_header_pointer_by_offset( offset_type const offset ) const
				{
					if ( offset >= holder_ptr_->size() ) {
						throw std::runtime_error( "Out of range." );
					}

					image::member_header const* const header =
						reinterpret_cast<image::member_header const* const>( holder_ptr_->data() + offset );


					if ( std::memcmp( header->end_header, image::end, sizeof( header->end_header ) ) != 0 ) {
						throw std::runtime_error( "Invalid archive end header." );
					}

					return header;
				}

				std::size_t get_archive_size_by_offset( offset_type const offset ) const
				{
					image::member_header const* const header = get_archive_header_pointer_by_offset( offset );

					return detail::binary_cast<std::size_t>( header->size );
				}

				// create instance
				archive_pointer_type get_archive_by_offset( offset_type const offset ) const
				{
					image::member_header const* const header = get_archive_header_pointer_by_offset( offset );

					auto const begin = holder_ptr_->data() + offset + sizeof( image::member_header );
					std::size_t const size = get_archive_size_by_offset( offset );
					archive_type h = {
						managed_member_header(
							header,
							[this]( char const* begin, char const* end )
							{
								if ( end - begin < 8  ) {	// neet at least 8bytes offset.
									throw std::runtime_error( "Invalid header name." );
								}
								return
									( ( *begin ) != '/' || ( ( *begin ) == '/' && *( begin + 1 ) == '/' ) )
									? std::string( begin, end )
									: longnames_.get_name_by_offset( detail::binary_cast<std::size_t>( begin + 1, end ) )
									;
							}
							),
						buffer_type( holder_ptr_, begin, begin + size )
					};

					return std::make_shared<archive_type>( std::move( h ) );
				}

				archive_pointer_type get_archive_by_offset_with_memoize( offset_type const offset ) const
				{
					if ( memo_.find( offset ) == memo_.end() ) {
						memo_[ offset ] = get_archive_by_offset( offset );
					}

					return memo_.at( offset );
				}

				// include special members
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
				holder_shared_pointer holder_ptr_;

				first_linker_member first_;
				second_linker_member second_;
				longnames longnames_;

				mutable std::unordered_map<offset_type, archive_pointer_type> memo_;
			};

			//
//			typedef basic_accessor<std::allocator> accessor;


/*			// read a binary from a file and build a object
			template<typename F, template <typename> class Allocator>
			basic_accessor<Allocator> build_object( F const& f, basic_accessor<Allocator>* )
			{
				auto p = std::make_shared<ytl::assembler::basic_binary<Allocator>>();
				f( *p );

				return basic_accessor<Allocator>( p );
			}*/

		} // namespace ar
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_AR_HPP*/