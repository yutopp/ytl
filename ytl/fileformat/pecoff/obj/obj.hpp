#ifndef YTL_FILEFORMAT_PECOFF_OBJ_HPP
#define YTL_FILEFORMAT_PECOFF_OBJ_HPP

#include <algorithm>

#include <boost/optional.hpp>

#include "../../config.hpp"
#include "../../binary_holder.hpp"

#include "../structure.hpp"

namespace ytl
{
	namespace fileformat
	{
		namespace pecoff
		{
			namespace obj
			{
				class reader
				{
				public:
					typedef std::vector<section const>	sections_type;
					typedef std::vector<symbol const>	symbols_type;

				public:
					template<typename CharT>
					reader( std::basic_string<CharT> const& filename )
						: holder_( filename )
					{
						construct();
					}

					template<typename IterT>
					reader( IterT const& begin, IterT const& end )
						: holder_( begin, end )
					{
						construct();
					}

					boost::optional<std::size_t> get_section_index( std::string const& name ) const
					{
						auto const it = find_symbol_position_byname( name );
						if ( it == symbols_.cend() ) {
							return boost::none;
						}

						return it->get_section_index();
					}

					inline boost::optional<section const&> find_section( std::string const& name ) const
					{
						return sections_.at( *get_section_index( name ) );
					}

					section const& get_section_at( std::size_t const index ) const
					{
						return sections_.at( index );
					};

					symbol const& get_symbol_at( std::size_t const index ) const
					{
						return symbols_.at( index );
					};

				private:
					void construct()
					{
						read_header();

						auto const section_begin =
							reinterpret_cast<::IMAGE_SECTION_HEADER const*>( header_ + 1 );
						auto const symbol_begin =
							reinterpret_cast<::IMAGE_SYMBOL const*>( holder_->data() + header_->PointerToSymbolTable );
						auto const long_symbol_tabel_begin =
							reinterpret_cast<char const*>( symbol_begin + header_->NumberOfSymbols );

						for( std::size_t i=0; i<header_->NumberOfSections; ++i )
							sections_.push_back( section( holder_, section_begin + i ) );

						for( std::size_t i=0; i<header_->NumberOfSymbols; ++i )
							symbols_.push_back( symbol( symbol_begin + i, long_symbol_tabel_begin ) );
					}

					void read_header()
					{
						if ( holder_->size() < sizeof(::IMAGE_FILE_HEADER) )
							throw std::exception();

						header_ = reinterpret_cast<::IMAGE_FILE_HEADER const*>( holder_->data() );
						std::cout << std::hex << std::endl
							<< "IMAGE_FILE_HEADER --" << std::endl
							<< "Machine : "					<< header_->Machine << std::endl
							<< "NumberOfSections : "		<< std::dec << header_->NumberOfSections << std::endl
							<< "TimeDateStamp : "			<< std::hex << "0x" << header_->TimeDateStamp << std::endl
							<< "PointerToSymbolTable : "	<< std::hex << "0x" << header_->PointerToSymbolTable << std::endl
							<< "NumberOfSymbols : "			<< std::dec << header_->NumberOfSymbols << std::endl
							<< "SizeOfOptionalHeader : "	<< std::dec << header_->SizeOfOptionalHeader << std::endl
							<< "Characteristics : "			<< std::hex << "0x" << header_->Characteristics << std::endl
							<< std::dec << std::endl;
					}

					void check_flags()
					{
						auto const flags = header_->Characteristics;
						if ( flags & IMAGE_FILE_RELOCS_STRIPPED )
							std::cout << "Option: " << "IMAGE_FILE_RELOCS_STRIPPED" << std::endl;
						if ( flags & IMAGE_FILE_EXECUTABLE_IMAGE )
							std::cout << "Option: " << "IMAGE_FILE_EXECUTABLE_IMAGE" << std::endl;	
						if ( flags & IMAGE_FILE_LINE_NUMS_STRIPPED )
							std::cout << "Option: " << "IMAGE_FILE_LINE_NUMS_STRIPPED" << std::endl;
						if ( flags & IMAGE_FILE_LOCAL_SYMS_STRIPPED )
							std::cout << "Option: " << "IMAGE_FILE_LOCAL_SYMS_STRIPPED" << std::endl;
					//	if ( flags & IMAGE_FILE_AGGRESSIVE_WS_TRIM )
					//		std::cout << "Option: " << "IMAGE_FILE_AGGRESSIVE_WS_TRIM" << std::endl;
						if ( flags & IMAGE_FILE_LARGE_ADDRESS_AWARE )
							std::cout << "Option: " << "IMAGE_FILE_LARGE_ADDRESS_AWARE" << std::endl;
						if ( flags & IMAGE_FILE_BYTES_REVERSED_LO )
							std::cout << "Option: " << "IMAGE_FILE_BYTES_REVERSED_LO" << std::endl;
						if ( flags & IMAGE_FILE_32BIT_MACHINE )
							std::cout << "Option: " << "IMAGE_FILE_32BIT_MACHINE" << std::endl;
						if ( flags & IMAGE_FILE_DEBUG_STRIPPED )
							std::cout << "Option: " << "IMAGE_FILE_DEBUG_STRIPPED" << std::endl;
						if ( flags & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP )
							std::cout << "Option: " << "IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP" << std::endl;
						if ( flags & IMAGE_FILE_NET_RUN_FROM_SWAP )
							std::cout << "Option: " << "IMAGE_FILE_NET_RUN_FROM_SWAP" << std::endl;
						if ( flags & IMAGE_FILE_SYSTEM )
							std::cout << "Option: " << "IMAGE_FILE_SYSTEM" << std::endl;
						if ( flags & IMAGE_FILE_DLL )
							std::cout << "Option: " << "IMAGE_FILE_DLL" << std::endl;
						if ( flags & IMAGE_FILE_UP_SYSTEM_ONLY )
							std::cout << "Option: " << "IMAGE_FILE_UP_SYSTEM_ONLY" << std::endl;
						if ( flags & IMAGE_FILE_BYTES_REVERSED_HI )
							std::cout << "Option: " << "IMAGE_FILE_BYTES_REVERSED_HI" << std::endl;
					}

					symbols_type::const_iterator find_symbol_position_byname( std::string const& name ) const
					{
						return std::find_if(
									symbols_.cbegin(),
									symbols_.cend(),
									[&]( symbol const& sec ) { return sec.get_name() == name; }
									);
					}

					binary_holder holder_;
					::IMAGE_FILE_HEADER const* header_;

					sections_type sections_;
					symbols_type symbols_;
				};

			} // namespace obj
		} // namespace pecoff
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_PECOFF_OBJ_HPP*/