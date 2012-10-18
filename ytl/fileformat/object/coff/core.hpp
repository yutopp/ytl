#ifndef YTL_FILEFORMAT_COFF_CORE_HPP
#define YTL_FILEFORMAT_COFF_CORE_HPP

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

#include "image.hpp"

namespace ytl
{
	namespace fileformat
	{
		namespace coff
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
						if ( buffer.size() < sizeof( image::file_header ) ) {
							throw std::runtime_error( "Invalid coff format." );
						}
					}
				};

				inline image::section_header const*
					section_header_begin_pointer( image::file_header const* const header )
				{
					return reinterpret_cast<image::section_header const*>( header + 1 );
				}

				inline image::symbol const*
					symbol_begin_pointer( image::file_header const* const header, byte_t const* const data )
				{
					return reinterpret_cast<image::symbol const*>( data + header->pointer_to_symbol_table );
				}

				inline byte_t const*
					long_symbol_table_begin_pointer( image::file_header const* const header, byte_t const* const data )
				{
					return reinterpret_cast<byte_t const*>( symbol_begin_pointer( header, data ) + header->number_of_symbols );
				}

			} // namespace op





			//
			class long_symbol_table
			{
				typedef std::size_t		offset_type;

			public:
				long_symbol_table( byte_t const* const buffer )
					: buffer_( buffer/* + sizeof( dword_t )*/ )
				{}

				std::string get_name_by_offset( offset_type const offset ) const
				{
					return std::string( reinterpret_cast<char const*>( buffer_ + offset ) );
				}

			private:
				byte_t const* buffer_;
			};


			// section information
			template<typename Buffer>
			class section
			{
			public:
				typedef Buffer												buffer_type;

			private:
				typedef std::vector<image::relocation>						relocation_table_type;
				typedef ytl::detail::container_copy_traits<buffer_type>		copy_traits;

			public:
				template<typename Holder>
				section(
					image::section_header const* const header,
					std::shared_ptr<Holder> const& buffer,
					typename buffer_type::const_pointer const data,
					std::size_t const number,
					long_symbol_table const& name_table
					)
					: self_( *header )
					, number_( number )
					, name_( header->name[0] != '/'
								? detail::shrink_space( header->name )
								: name_table.get_name_by_offset( detail::binary_cast<std::size_t>( header->name + 1, header->name + sizeof( header->name ) ) )
						)
				{
					auto const flags = header->characteristics;

//					std::cout
//						<< "!! Section // no : " << number << std::endl
//						<< "section_header_name: " << name_ << " size -> " << name_.size() << std::endl
//						<< "SizeOfRawData: " << std::dec << header->size_of_raw_data << std::endl
//						<< "PointerToRawData: 0x" << std::hex << header->pointer_to_raw_data << std::endl
//						<< "Characteristics: " << std::dec << header->characteristics << std::endl;

					//
					if ( header->size_of_raw_data != 0 ) {
						auto const buffer_begin = data + header->pointer_to_raw_data;

						binary_ = copy_traits::copy( buffer, buffer_begin, buffer_begin + header->size_of_raw_data );
					}

					// 
					auto const relocations_begin = reinterpret_cast<image::relocation const*>( data + header->pointer_to_relocations );
					auto const relocations_num = ( flags & image::scn_lnk_nreloc_ovfl )
													? relocations_begin->reloc_count - 1/*(?)*/	// IMAGE_SCN_LNK_NRELOC_OVFL ‚ª —§‚Á‚Ä‚¢‚éê‡‚ÍAÄ”z’uî•ñ‚Ìæ“ª‚ªŒÂ”‚Ìî•ñŽ‚Â
													: header->number_of_relocations;
					for( std::size_t i=( flags & image::scn_lnk_nreloc_ovfl ) ? 1 : 0; i<relocations_num; ++i ) {	
						relocations_.emplace_back( *( relocations_begin + i ) );
					}

					std::cout << "relocations_num : " << relocations_num << std::endl;
					std::cout << std::dec <<std::endl;
				}

				std::size_t get_number() const
				{
					return number_;
				}

				std::string const& get_name() const
				{
					return name_;
				}

				buffer_type const& get_binary() const
				{
					return binary_;
				}

				dword_t get_characteristics() const
				{
					return self_.characteristics;
				}


				relocation_table_type const& get_relocations() const
				{
					return relocations_;
				}

			private:
				image::section_header self_;
				std::size_t number_;
				std::string name_;
				buffer_type binary_;

				relocation_table_type relocations_;
			};


			// 
			template<typename Buffer>
			class sections
			{
			public:
				typedef Buffer											buffer_type;

				typedef section<buffer_type>							section_type;
				typedef std::vector<section_type>						section_table_type;

				typedef typename section_table_type::const_iterator		const_iterator;
				typedef typename section_table_type::size_type			size_type;

			public:
				template<typename Holder>
				sections(
					image::file_header const* const header,
					image::section_header const* const begin_section_headers_pointer,
					std::shared_ptr<Holder const> const& holder,
					long_symbol_table const& name_table
					)
				{
					std::cout << "Sections_num : " << header->number_of_sections << std::endl;

					for( std::size_t i=0; i<header->number_of_sections; ++i ) {
						sections_.emplace_back( begin_section_headers_pointer + i, holder, holder->data(), i, name_table );
					}
				}



				section_type const& at( size_type const index ) const
				{
					return sections_.at( index - 1 );		// 1 - based
				}

				const_iterator begin() const
				{
					return sections_.begin();
				}

				const_iterator end() const
				{
					return sections_.end();
				}

				size_type size() const
				{
					return sections_.size();
				}

			private:
				section_table_type sections_;
			};



			// symbol information
			class symbol
			{
			public:
				symbol( image::symbol const* const body, long_symbol_table const& name_table )
					: self_( *body )
				{
//					std::cout << "section_number: " << body->section_number << std::endl;
//					if ( body->section_number < 1 )	// if...
//						throw std::runtime_error( "Invalid symbol's section number." );

					// if p->N.Name.Short is zero, use name_table
					name_ = ( body->N.name.p_short == 0 )
								? name_table.get_name_by_offset( body->N.name.p_long )
								: detail::shrink_space( body->N.short_name );

					// 
					for( std::size_t i=0; i<body->number_of_aux_symbols; ++i ) {
						aux_raw_symbols_.emplace_back( *( body + i ) );
					}

					//
/*					std::cout
						<< "is_short: " << ( body->N.name.p_short == 0 ) << "; offset : " << body->N.name.p_long << std::endl
						<< "symbol_name: " << name_ << std::endl
						<< "SectionNumber: " << (int)body->section_number << std::endl
						<< "StorageClass: " << (int)body->storage_class << std::endl
						<< "Value: " << (int)body->value << std::endl
						<< "Aux : " << (int)body->number_of_aux_symbols << std::endl
						<< std::endl;*/
				}


				std::string const& get_name() const
				{
					return name_;
				}

				short_t get_section_number() const
				{
					return self_.section_number;
				}

				byte_t storage_class() const
				{
					return self_.storage_class;
				}

				word_t type() const
				{
					return self_.type;
				}

				dword_t value() const
				{
					return self_.value;
				}

				std::vector<image::symbol> const& aux_raw_symbols() const
				{
					return aux_raw_symbols_;
				}

			private:
				image::symbol self_;

				std::string name_;
				std::vector<image::symbol> aux_raw_symbols_;
			};






			class symbols
			{
			public:
				typedef symbol									symbol_type;
				typedef std::vector<symbol_type>				symbol_table_type;

				typedef symbol_table_type::const_iterator		const_iterator;
				typedef symbol_table_type::size_type			size_type;

			public:
				symbols(
					image::file_header const* const header,
					image::symbol const* const begin_symbol_pointer,
					long_symbol_table const& name_table
					)
				{
//					std::cout << "Symbols_num : " << header->number_of_symbols << std::endl
//						<< (void const*)&header << " : " << (void*)begin_symbol_pointer << std::endl;

					for( std::size_t i=0; i<header->number_of_symbols; ++i ) {
						auto const p = begin_symbol_pointer + i;
						symbols_.emplace_back( p, name_table );

//						i += p->number_of_aux_symbols;
					}
				}


				symbol const& at( size_type const index ) const
				{
					std::cout << "?!! require symbol index : " << index << " / " << symbols_.size() << std::endl;
					return symbols_.at( index );
				}

				const_iterator begin() const
				{
					return symbols_.begin();
				}

				const_iterator end() const
				{
					return symbols_.end();
				}

				size_type size() const
				{
					return symbols_.size();
				}



				bool is_exist( std::string const& name ) const
				{
					return find( name ) != symbols_.cend();
				}

				boost::optional<symbol_type const&> get( std::string const& name ) const
				{
					auto const it = find( name );
					if ( it == symbols_.cend() ) {
						return boost::none;
					}

					return *it;
				}

				const_iterator find( std::string const& name ) const
				{
					return std::find_if(
								symbols_.cbegin(),
								symbols_.cend(),
								[&]( symbol_type const& sec ) { return sec.get_name() == name; }
								);
				}

			private:
				symbol_table_type symbols_;
			};



			class immutable_accessor
			{
				friend op::validator;

			private:
				typedef detail::immutable_valid_buffer_holder<op::validator>	holder_type;
				typedef holder_type const										const_holder_type;
				typedef std::shared_ptr<const_holder_type>						holder_shared_pointer;

				typedef const_shared_binary_range<const_holder_type>			buffer_type;

			public:
				typedef sections<buffer_type>									section_table_type;
				typedef section_table_type::section_type				section_type;
				typedef	section_type const&										const_section_reference;
				typedef boost::optional<const_section_reference>				optional_const_section_reference;

				typedef symbols													symbol_table_type;
				typedef symbol_table_type::symbol_type							symbol_type;
				typedef	symbol_type const&										const_symbol_reference;
				typedef boost::optional<const_symbol_reference>					optional_const_symbol_reference;

			public:
				// move / copy / unique_ptr
				template<typename Buffer>
				immutable_accessor( Buffer&& buffer )
					: holder_ptr_( std::make_shared<holder_type>( std::forward<Buffer>( buffer ) ) )
					, header_( reinterpret_cast<image::file_header const*>( holder_ptr_->data() ) )
					, long_names_(
							op::long_symbol_table_begin_pointer( header_, holder_ptr_->data() )
							)
					, sections_(
							header_,
							op::section_header_begin_pointer( header_ ),
							holder_ptr_,
							long_names_
							)
					, symbols_(
							header_,
							op::symbol_begin_pointer( header_, holder_ptr_->data() ),
							long_names_
							)
				{}

			public:
				bool is_exist( std::string const& name ) const
				{
					return static_cast<bool>( get_section( name ) );
				}
				
				optional_const_section_reference get_section( std::string const& name ) const
				{
					if ( auto const& sym = symbols_.get( name ) ) {
						return get_section( *sym );
					}

					return boost::none;
				}

				const_section_reference get_section( symbol_type const& symbol ) const
				{
					return sections_.at( symbol.get_section_number() );
				}

				optional_const_symbol_reference get_symbol( std::string const& name ) const
				{
					return symbols_.get( name );
				}

				section_table_type const& get_sections() const
				{
					return sections_;
				}

				symbol_table_type const& get_symbols() const
				{
					return symbols_;
				}

			private:
				holder_shared_pointer holder_ptr_;
				image::file_header const* header_;

				long_symbol_table long_names_;
				section_table_type sections_;
				symbol_table_type symbols_;
			};


			template<typename OutputBuffer>
			class basic_accessor
			{
				/* Not implemented */
			};


		} // namespace coff
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_COFF_CORE_HPP*/