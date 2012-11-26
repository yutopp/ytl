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
                    template<typename Buffer>
                    void operator()( Buffer const& buffer ) const
                    {
                        if ( buffer.size() < sizeof( image::file_header ) ) {
                            throw std::domain_error( "Invalid coff format." );
                        }

                        image::file_header const* const header
                            = reinterpret_cast<image::file_header const*>( buffer.data() );

                        if ( !is_valid_machine_type( header->machine ) ) {
                            throw std::domain_error( "Invalid coff format." );
                        }

                        if ( buffer.size() < header->pointer_to_symbol_table ) {
                            throw std::domain_error( "Invalid coff format." );
                        }
                    }

                private:
                    bool is_valid_machine_type( word_t const type ) const
                    {
                        switch( type )
                        {
                        case image::file_machine_unknown:
                        case image::file_machine_i386:
                            break;

                        default:
                            return false;
                        }

                        return true;
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
					long_symbol_name_table_begin_pointer( image::file_header const* const header, byte_t const* const data )
				{
					return reinterpret_cast<byte_t const*>( symbol_begin_pointer( header, data ) + header->number_of_symbols );
				}

			} // namespace op


			//
            class long_symbol_name_table
            {
                typedef std::size_t             size_type, offset_type;
                typedef std::string             string_type;

            public:
                template<typename HolderPtr>
                long_symbol_name_table(
                    HolderPtr const& holder,
                    image::file_header const* const header
                    )
                {
                    byte_t const* const buffer = op::long_symbol_name_table_begin_pointer( header, holder->data() );
                    size_type const size = *reinterpret_cast<dword_t const*>( buffer );

                    for( size_t i=0, offset=0; i<size; ++i ) {
                        string_type name( reinterpret_cast<string_type::value_type const*>( buffer ) + offset );
                        auto const length = name.size();

                        names_.emplace( offset, std::move( name ) );
                        offset += length;
                    }
                }

                std::string get_name_by_offset( offset_type const offset ) const
                {
                    return names_.at( offset );
                }

            private:
                std::map<offset_type, string_type> names_;
            };


            //
            template<typename HolderPtr>
            class lazy_long_symbol_name_table
            {
                typedef HolderPtr               holder_shared_pointer;
                typedef std::size_t             size_type, offset_type;

            public:
                lazy_long_symbol_name_table(
                    holder_shared_pointer holder,
                    image::file_header const* const header
                    )
                    : holder_( holder )
                    , buffer_( op::long_symbol_name_table_begin_pointer( header, holder->data() ) )
                    , size_( *reinterpret_cast<size_type const*>( buffer_ ) )
                {}

                std::string get_name_by_offset( offset_type const offset ) const
                {
                    return std::string( reinterpret_cast<char const*>( buffer_ + offset ) );
                }

            private:
                holder_shared_pointer holder_;
                byte_t const* buffer_;
                size_type size_;
            };




            // section information
            template<typename Buffer>
            class section
            {
            public:
                typedef Buffer                                              buffer_type;
                typedef image::relocation                                   relocation_type;
                typedef std::vector<relocation_type>                        relocation_table_type;

            private:
                typedef ytl::detail::container_copy_traits<buffer_type>     copy_traits;

            public:
                template<typename HolderPtr, typename LongNameTablePtr>
                section(
                    HolderPtr const& holder,
                    image::section_header const* const header,
                    std::size_t const index,
                    LongNameTablePtr const& name_table
                    )
                    : self_( *header )
                    , number_( index )
                    , name_( header->name[0] != '/'
                                ? detail::trim( header->name )
                                : name_table->get_name_by_offset(
                                    detail::binary_cast<std::size_t>( header->name + 1, header->name + sizeof( header->name ) )
                                    )
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
						auto const buffer_begin = holder->data() + header->pointer_to_raw_data;

						binary_ = copy_traits::copy(
                            holder,
                            buffer_begin,
                            buffer_begin + header->size_of_raw_data
                            );
					}

					// 
					auto const relocations_begin = reinterpret_cast<image::relocation const*>( holder->data() + header->pointer_to_relocations );
					auto const relocations_num = ( flags & image::scn_lnk_nreloc_ovfl )
													? relocations_begin->reloc_count	// IMAGE_SCN_LNK_NRELOC_OVFL ‚ª —§‚Á‚Ä‚¢‚éê‡‚ÍAÄ”z’uî•ñ‚Ìæ“ª‚ªŒÂ”‚Ìî•ñŽ‚Â
													: header->number_of_relocations;
					auto const relocations_offset = ( flags & image::scn_lnk_nreloc_ovfl )
													? 1
													: 0;

					relocations_.reserve( relocations_num - relocations_offset );
					for( std::size_t i=relocations_offset ? 1 : 0; i<relocations_num; ++i ) {	
						relocations_.emplace_back( *( relocations_begin + i ) );
					}

//					std::cout << "relocations_num : " << relocations_num << std::endl;
//					std::cout << std::dec;
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
			template<typename Buffer, typename HolderPtr, typename LongNameTablePtr>
			class lazy_sections
			{
			public:
				typedef Buffer											buffer_type;
				typedef HolderPtr										holder_shared_pointer;
				typedef LongNameTablePtr								long_symbol_table_shared_pointer;

				typedef section<buffer_type>							section_type;
				typedef std::vector<section_type>						section_table_type;

				typedef typename section_table_type::size_type			size_type;

			public:
				lazy_sections(
					image::file_header const* const header,
					holder_shared_pointer const& holder,
					long_symbol_table_shared_pointer const& name_table
					)
					: holder_( holder )
					, long_name_table_( name_table )
					, sections_num_( header->number_of_sections )
					, section_begin_( op::section_header_begin_pointer( header ) )
				{
					std::cout << "Sections_num : " << sections_num_ << std::endl;
				}

				section_type const& at( size_type const index ) const
				{
					return section_type(
						section_begin_ + ( i - 1 ),		// 1 - based
						holder,
						holder->data(),
						i,
						name_table
						);
				}

				size_type size() const
				{
					return sections_num_;
				}

			private:
				holder_shared_pointer holder_;
				long_symbol_table_shared_pointer long_name_table_;

				std::size_t sections_num_;
				image::section_header const* section_begin_;
			};



            // 
            template<typename Buffer>
            class sections
            {
            public:
                typedef Buffer                                              buffer_type;

                typedef section<buffer_type>                                section_type;
                typedef std::vector<section_type>                           section_table_type;

                typedef typename section_table_type::iterator               iterator;
                typedef typename section_table_type::const_iterator         const_iterator;
                typedef typename section_table_type::size_type              size_type;

            private:
                typedef ytl::detail::container_copy_traits<buffer_type>     copy_traits;

			public:
				template<typename HolderPtr, typename LongNameTablePtr>
				sections(
                    HolderPtr const& holder,
                    image::file_header const* const header,
                    image::section_header const* const section_headers_begin_pointer,
                    LongNameTablePtr const& name_table
					)
                    : total_binary_(
                        copy_traits::copy(
                            holder,
                            reinterpret_cast<byte_t const*>( section_headers_begin_pointer + header->number_of_sections ),
                            reinterpret_cast<byte_t const*>( op::symbol_begin_pointer( header, holder->data() ) )
                            )
                        )
				{
					std::cout << "Sections_num : " << header->number_of_sections << std::endl;

					for( std::size_t i=0; i<header->number_of_sections; ++i ) {
						sections_.emplace_back( holder, section_headers_begin_pointer + i, i, name_table );
					}

					assert( sections_.size() == header->number_of_sections );
				}

                buffer_type const& test( std::size_t const offset ) const
                {
                    return total_binary_;
/*                    total_binary_.data()
                    std::cout
                        << "index: " << offset / sizeof( image::section ) << std::endl
                        << "mod  : " << offset % sizeof( image::section ) << std::endl;*/
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
                buffer_type total_binary_;
				section_table_type sections_;
			};



			// symbol information
			class symbol
			{
			public:
				template<typename LongNameTablePtr>
				symbol(
					image::symbol const* const body,
					LongNameTablePtr const& name_table
					)
					: self_( *body )
				{
//					std::cout << "section_number: " << body->section_number << std::endl;
//					if ( body->section_number < 1 )	// if...
//						throw std::runtime_error( "Invalid symbol's section number." );

					// if p->N.Name.Short is zero, use name_table
					name_ = ( body->N.name.p_short == 0 )
								? name_table->get_name_by_offset( body->N.name.p_long )
								: detail::trim( body->N.short_name );

					// 
					for( std::size_t i=0; i<body->number_of_aux_symbols; ++i ) {
						aux_raw_symbols_.emplace_back( *( body + i ) );
					}

					//
//					std::cout
//						<< "is_short: " << ( body->N.name.p_short == 0 ) << "; offset : " << body->N.name.p_long << std::endl
//						<< "symbol_name: \"" << name_ << "\"" << std::endl
//						<< "SectionNumber: " << (int)body->section_number << std::endl
//						<< "StorageClass: " << (int)body->storage_class << std::endl
//						<< "Value: " << (int)body->value << std::endl
//						<< "Aux : " << (int)body->number_of_aux_symbols << std::endl
//						<< std::endl;/**/
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

				typedef symbol_table_type::iterator				iterator;
				typedef symbol_table_type::const_iterator		const_iterator;
				typedef symbol_table_type::size_type			size_type;

			public:
				template<typename HolderPtr, typename LongNameTablePtr>
				symbols(
					image::file_header const* const header,
					HolderPtr const& holder,
					LongNameTablePtr const& name_table
					)
				{
					image::symbol const* const begin_symbol_pointer
						= op::symbol_begin_pointer( header, holder->data() );
//					std::cout << "Symbols_num : " << header->number_of_symbols << std::endl
//						<< (void const*)&header << " : " << (void*)begin_symbol_pointer << std::endl;

					for( std::size_t i=0; i<header->number_of_symbols; ++i ) {
						symbols_.emplace_back( begin_symbol_pointer + i, name_table );
					}
				}

			public:
				symbol const& at( size_type const index ) const
				{
//					std::cout << "?!! require symbol index : " << index << " / " << symbols_.size() << std::endl;
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
//               : private boost::noncopyable
			{
            public:
				typedef op::validator                                           validator_type;

			private:
				typedef detail::immutable_valid_buffer_holder<validator_type>	holder_type;
				typedef holder_type const										const_holder_type;
				typedef std::shared_ptr<const_holder_type>						holder_shared_pointer;

			public:
				typedef const_shared_binary_range<const_holder_type>			buffer_type;

				typedef lazy_long_symbol_name_table<holder_shared_pointer>		long_symbol_name_table_type;
				typedef std::shared_ptr<long_symbol_name_table_type const>		long_symbol_name_table_shared_pointer;

				typedef sections<buffer_type>									section_table_type;
				typedef section_table_type::section_type						section_type;
				typedef	section_type const&										const_section_reference;
				typedef boost::optional<const_section_reference>				optional_const_section_reference;

				typedef symbols													symbol_table_type;
				typedef symbol_table_type::symbol_type							symbol_type;
				typedef	symbol_type const&										const_symbol_reference;
				typedef boost::optional<const_symbol_reference>					optional_const_symbol_reference;

			public:
				// generate from buffer - move / copy / unique_ptr
				template<typename Buffer>
				explicit immutable_accessor( Buffer&& buffer, YTL_ENABLE_IF_ONLY_BUFFER_TYPE( Buffer ) )
					: holder_ptr_( std::make_shared<holder_type>( std::forward<Buffer>( buffer ) ) )
					, header_( reinterpret_cast<image::file_header const*>( holder_ptr_->data() ) )
					, long_names_( std::make_shared<long_symbol_name_table_type>( holder_ptr_, header_ ) )
                    , sections_(
                            holder_ptr_,
                            header_,
                            op::section_header_begin_pointer( header_ ),
                            long_names_
                            )
					, symbols_(
							header_,
							holder_ptr_,
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

				long_symbol_name_table_shared_pointer long_names_;
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