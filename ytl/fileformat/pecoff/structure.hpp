#ifndef YTL_FILEFORMAT_PECOFF_STRUCTURE_HPP
#define YTL_FILEFORMAT_PECOFF_STRUCTURE_HPP

#include <vector>
#include <string>
#include <iostream>

#include "../config.hpp"
#include "../binary_holder.hpp"

#include <Windows.h>

namespace ytl
{
	namespace fileformat
	{
		namespace pecoff
		{
			// relocation information
			class relocation
			{
			public:
				relocation( ::IMAGE_RELOCATION const* p )
					: reloc_( *p )
				{}

				::IMAGE_RELOCATION const& get() const
				{
					return reloc_;
				}

			private:
				::IMAGE_RELOCATION const reloc_;
			};


			// section information
			class section
			{
			public:
				typedef std::vector<relocation>	relocation_table_type;

			public:
				section( binary_holder const& bin, ::IMAGE_SECTION_HEADER const* p )
				{
					std::string const short_name( std::string( p->Name, p->Name + sizeof( p->Name ) ).c_str() );
					auto const flags = p->Characteristics;

					std::cout
						<< "section_name: " << short_name << " size -> " << short_name.size() << std::endl
						<< "SizeOfRawData: " << std::dec << p->SizeOfRawData << std::endl
						<< "PointerToRawData: 0x" << std::hex << p->PointerToRawData << std::endl
						<< "Characteristics: " << std::dec << p->Characteristics << std::endl;

					//
					if ( p->SizeOfRawData != 0 ) {
						raw_data_ =
							binary_holder( bin->data() + p->PointerToRawData, bin->data() + p->PointerToRawData + p->SizeOfRawData );
					}

					// 
					::IMAGE_RELOCATION const* const begin =
						reinterpret_cast<::IMAGE_RELOCATION const*>( bin->data() + p->PointerToRelocations );
					std::size_t const relocations_num =				// IMAGE_SCN_LNK_NRELOC_OVFL が 立っている場合は、再配置情報の先頭が個数の情報を
						( flags & IMAGE_SCN_LNK_NRELOC_OVFL ) ? begin->RelocCount : p->NumberOfRelocations;	// 持っているため、とりあえず1つにしておく。
					for( std::size_t i=( flags & IMAGE_SCN_LNK_NRELOC_OVFL ) ? 1 : 0; i<relocations_num; ++i ) {	
						relocations_.push_back( relocation( begin + i ) );
					}

					std::cout << "relocations_num : " << relocations_num << std::endl;
					std::cout << std::dec <<std::endl;
				}

				binary_holder const& get_raw_binary() const
				{
					return raw_data_;
				}

				relocation_table_type const& get_relocations() const
				{
					return relocations_;
				}

			private:
				std::string name_;
				binary_holder raw_data_;
				relocation_table_type relocations_;
			};


			// symbol information
			class symbol
			{
			public:
				symbol( ::IMAGE_SYMBOL const* p, char const* name_table )
					: section_index_( 0 )
				{
					if ( p->SectionNumber < 1 )	// 仮に
						return;

					// if p->N.Name.Short is zero, use name_table
					name_ = std::string(
							( p->N.Name.Short == 0 ) ?
							name_table + p->N.Name.Long :
							std::string( p->N.ShortName, p->N.ShortName + sizeof( p->N.ShortName ) ).c_str()
							);
		
					section_index_ = p->SectionNumber - 1;	// Because, SectionNumber is 1-based.
					//
					std::cout
						<< "is_short: " << ( p->N.Name.Short == 0 ) << "; offset : " << p->N.Name.Long << std::endl
						<< "symbol_name: " << name_ << std::endl
						<< "SectionNumber: " << p->SectionNumber << std::endl
						<< "Aux : " << (int)p->NumberOfAuxSymbols << std::endl
						<< std::endl;
				}

				std::string const& get_name() const
				{
					return name_;
				}

				int get_section_index() const
				{
					return section_index_;
				}

			private:
				std::string name_;
				int section_index_;
			};

		} // namspace pecoff
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_PECOFF_STRUCTURE_HPP*/