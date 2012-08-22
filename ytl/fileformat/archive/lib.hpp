#ifndef YTL_FILEFORMAT_ARCHIVE_LIB_HPP
#define YTL_FILEFORMAT_ARCHIVE_LIB_HPP

#include <cstddef>
#include <ctime>

#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include <ytl/assembler.hpp>

#include <windows.h>

#include "../config.hpp"

namespace ytl
{
	namespace fileformat
	{
		namespace archive
		{
			namespace lib
			{
				// data structure
				struct archive
				{
					std::string name;
					std::time_t date;
					ytl::assembler::binary_holder<> value;
				};


				//
				namespace detail
				{
					// 
					class second_linker_member
					{
					public:
						second_linker_member( archive const& ar )
						{
							//
							unsigned long const member_num =
								*reinterpret_cast<unsigned long const*>( &(*ar.value)[0] );
							std::cout << "member : " << member_num << std::endl;

							//
							for( unsigned long i=0; i<member_num; ++i ) {
								unsigned long const offset =
									*( reinterpret_cast<unsigned long const*>( &(*ar.value)[0] ) + i + 1 );
								std::cout << "offset : " << offset << std::endl;
								offsets_.push_back( offset );
							}

							//
							unsigned long const symbol_num =
								*( reinterpret_cast<unsigned long const*>( &(*ar.value)[0] ) + member_num + 1 );
							std::cout << "symbol_num : " << symbol_num << std::endl;

							//
							unsigned short const* pointer =
								reinterpret_cast<unsigned short const*>(
									reinterpret_cast<unsigned long const*>( &(*ar.value)[0] ) + member_num + 2
									);
							for( unsigned long i=0; i<symbol_num; ++i ) {
								unsigned short const index = *( pointer + i );
								std::cout << "symbol : " << index << std::endl;
								indices_.push_back( index );
							}

							//
							char const* it =
								reinterpret_cast<char const*>( pointer + symbol_num );
							for( std::size_t i=0; i<symbol_num; ++i ) {
								std::string name( it );
								it += name.size() + 1;

								std::cout << "name : " << name << std::endl;

								symbols_.push_back( std::move( name ) );	
							}
						}

						boost::optional<unsigned long> find_symbol( std::string const& symbol ) const
						{
							auto const it = std::lower_bound( symbols_.cbegin(), symbols_.cend(), symbol );
							if ( it == symbols_.cend() )
								return boost::none;

							std::size_t const index = std::distance( symbols_.cbegin(), it );
							return offsets_.at( indices_.at( index ) - 1 );	// symbol's index is 1-based.
						}

					private:
						std::vector<unsigned long> offsets_;
						std::vector<unsigned short> indices_;
						std::vector<std::string> symbols_;
					};


					// 
					class long_name_table
					{
					public:

					private:
						std::vector<std::string> names_;
					};


					//
					class body
					{
					public:
						body( std::string const& filename )
							: binary_( filename )
						{
							validate();
						}

						bool is_loaded() const
						{
							return !binary_->empty();
						}

						operator bool() const
						{
							return is_loaded();
						}

						archive get_archive_at( std::size_t const index ) const
						{
							std::size_t offset = IMAGE_ARCHIVE_START_SIZE;

							for( std::size_t i=0; i<index; ++i ) {
								offset +=
									sizeof(::IMAGE_ARCHIVE_MEMBER_HEADER)
									+ get_archive_size_from_offset( offset );

								// lib file format is 2Byte aliginment.
								if ( offset % 2 != 0 ) {
									++offset;
								}
							}

							auto ar = get_archive_from_offset( offset );
						
							std::cout
								<< "name : " << ar.name << "\n"
								<< "date : " << ar.date << "\n"
								<< "size : " << ar.value->size() << "\n"
								<< std::endl;

							return ar;
						}

						//
						archive get_archive_from_offset( std::size_t const offset ) const
						{
							::IMAGE_ARCHIVE_MEMBER_HEADER const* const header =
								reinterpret_cast<::IMAGE_ARCHIVE_MEMBER_HEADER const*>( binary_->data() + offset );

							std::string const name_context( header->Name, header->Name + sizeof( header->Name ) );

							std::string const date_context( header->Date, header->Date + sizeof( header->Date ) );
							std::time_t const date =
								boost::lexical_cast<std::time_t>( boost::algorithm::replace_all_copy( date_context, " ", "" ) );

							std::size_t const size = get_archive_size_from_offset( offset ); // with footer check
		
							archive result = {
								name_context,
								date,
								ytl::assembler::binary_holder<>(
									binary_->data() + offset + sizeof( IMAGE_ARCHIVE_MEMBER_HEADER ),
									binary_->data() + offset + sizeof( IMAGE_ARCHIVE_MEMBER_HEADER ) + size
									)
							};
							return result;
						}

					private:
						// validate header
						void validate() const
						{
							if ( binary_->size() < IMAGE_ARCHIVE_START_SIZE )
								throw std::exception( "Invalid lib format." );

							if ( std::string( binary_->cbegin(), binary_->cbegin() + IMAGE_ARCHIVE_START_SIZE ) != IMAGE_ARCHIVE_START )
								throw std::exception( "Invalid header." );
						}

						//
						std::size_t get_archive_size_from_offset( std::size_t const offset ) const
						{
							::IMAGE_ARCHIVE_MEMBER_HEADER const* const header =
								reinterpret_cast<::IMAGE_ARCHIVE_MEMBER_HEADER const*>( binary_->data() + offset );

							std::string const size_context( header->Size, header->Size + sizeof( header->Size ) );
							std::size_t const size =
								boost::lexical_cast<std::size_t>( boost::algorithm::replace_all_copy( size_context, " ", "" ) );

							std::string const end_header_context( header->EndHeader, header->EndHeader + sizeof( header->EndHeader ) );
							if ( end_header_context != IMAGE_ARCHIVE_END )
								throw std::exception( "Invalid archive end header." );

							return size;
						}
						
						ytl::assembler::binary_holder<> binary_;
					};

				} // namespace detail



				class reader
				{
				public:
					reader( std::string const& filename )
						: body_( filename )
						, function_table_( body_.get_archive_at( 1 ) )	// index 1 is second linker member information.
					{}

					boost::optional<archive> find_symbol( std::string const& symbol )
					{
						if ( auto const offset = function_table_.find_symbol( symbol ) )
							return body_.get_archive_from_offset( *offset );

						return boost::none;
					}

				private:
					detail::body body_;
					detail::second_linker_member function_table_;
				};

			} // namespace lib
		} // namespace archive
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_ARCHIVE_LIB_HPP*/