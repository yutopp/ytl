#ifndef YTL_FILEFORMAT_COFF_IMAGE_HPP
#define YTL_FILEFORMAT_COFF_IMAGE_HPP

#include "../../../config.hpp"

namespace ytl
{
	namespace fileformat
	{
		namespace coff
		{
			namespace image
			{
				std::size_t const sizeof_file_header		= 20;

				enum file_header_characteristics : word_t
				{
				};
				word_t const file_machine_unknown			= 0;
				word_t const file_machine_I386				= 0x014c;	// Intel 386.
				word_t const file_machine_R3000				= 0x0162;	// MIPS little-endian, 0x160 big-endian
				word_t const file_machine_R4000				= 0x0166;	// MIPS little-endian
				word_t const file_machine_R10000			= 0x0168;	// MIPS little-endian
				word_t const file_machine_WCEMIPSV2			= 0x0169;	// MIPS little-endian WCE v2
				word_t const file_machine_ALPHA				= 0x0184;	// Alpha_AXP
				word_t const file_machine_SH3				= 0x01a2;	// SH3 little-endian
				word_t const file_machine_SH3DSP			= 0x01a3;	
				word_t const file_machine_SH3E				= 0x01a4;	// SH3E little-endian
				word_t const file_machine_SH4				= 0x01a6;	// SH4 little-endian
				word_t const file_machine_SH5				= 0x01a8;	// SH5
				word_t const file_machine_ARM				= 0x01c0;	// ARM Little-Endian
				word_t const file_machine_THUMB				= 0x01c2;	// ARM Thumb/Thumb-2 Little-Endian
				word_t const file_machine_ARMNT				= 0x01c4;	// ARM Thumb-2 Little-Endian
				word_t const file_machine_AM33				= 0x01d3;	
				word_t const file_machine_POWERPC			= 0x01F0;	// IBM PowerPC Little-Endian
				word_t const file_machine_POWERPCFP			= 0x01f1;	
				word_t const file_machine_IA64				= 0x0200;	// Intel 64
				word_t const file_machine_MIPS16			= 0x0266;	// MIPS
				word_t const file_machine_ALPHA64			= 0x0284;	// ALPHA64
				word_t const file_machine_MIPSFPU			= 0x0366;	// MIPS
				word_t const file_machine_MIPSFPU16			= 0x0466;	// MIPS
				word_t const file_machine_AXP64				= file_machine_ALPHA64;
				word_t const file_machine_TRICORE			= 0x0520;	// Infineon
				word_t const file_machine_CEF				= 0x0CEF;	
				word_t const file_machine_EBC				= 0x0EBC;	// EFI Byte Code
				word_t const file_machine_AMD64				= 0x8664;	// AMD64 (K8)
				word_t const file_machine_M32R				= 0x9041;	// M32R little-endian
				word_t const file_machine_CEE				= 0xC0EE;	

				word_t const file_relocs_stripped			= 0x0001;	// Relocation info stripped from file.
				word_t const file_executable_image			= 0x0002;	// File is executable  (i.e. no unresolved externel references).
				word_t const file_line_nums_stripped		= 0x0004;	// Line nunbers stripped from file.
				word_t const file_local_syms_stripped		= 0x0008;	// Local symbols stripped from file.
				word_t const file_aggresive_ws_trim			= 0x0010;	// Agressively trim working set
				word_t const file_large_address_aware		= 0x0020;	// App can handle >2gb addresses
				word_t const file_bytes_reversed_lo			= 0x0080;	// Bytes of machine word are reversed.
				word_t const file_32bit_machine				= 0x0100;	// 32 bit word machine.
				word_t const file_debug_stripped			= 0x0200;	// Debugging info stripped from file in .DBG file
				word_t const file_removable_run_from_swap	= 0x0400;	// If Image is on removable media, copy and run from the swap file.
				word_t const file_net_run_from_swap			= 0x0800;	// If Image is on Net, copy and run from the swap file.
				word_t const file_system					= 0x1000;	// System File.
				word_t const file_dll						= 0x2000;	// File is a DLL.
				word_t const file_up_system_only			= 0x4000;	// File should only be run on a UP machine
				word_t const file_bytes_reversed_hi			= 0x8000;	// Bytes of machine word are reversed.

				struct file_header
				{
					word_t machine;
					word_t number_of_sections;
					dword_t time_date_stamp;
					dword_t pointer_to_symbol_table;
					dword_t number_of_symbols;
					word_t size_of_optional_header;
					word_t characteristics;
				};




				// I386 relocation types.
				word_t const rel_I386_absolute			= 0x0000;	// Reference is absolute, no relocation is necessary
				word_t const rel_I386_dir16				= 0x0001;	// Direct 16-bit reference to the symbols virtual address
				word_t const rel_I386_rel16				= 0x0002;	// PC-relative 16-bit reference to the symbols virtual address
				word_t const rel_I386_dir32				= 0x0006;	// Direct 32-bit reference to the symbols virtual address
				word_t const rel_I386_dir32nb			= 0x0007;	// Direct 32-bit reference to the symbols virtual address, base not included
				word_t const rel_I386_seg12				= 0x0009;	// Direct 16-bit reference to the segment-selector bits of a 32-bit virtual address
				word_t const rel_I386_section			= 0x000A;	
				word_t const rel_I386_secrel			= 0x000B;	
				word_t const rel_I386_token				= 0x000C;	// clr token
				word_t const rel_I386_secrel7			= 0x000D;	// 7 bit offset from base of section containing target
				word_t const rel_I386_rel32				= 0x0014;	// PC-relative 32-bit reference to the symbols virtual address

#include <ytl/detail/packed_2_begin.hpp>
				struct relocation
				{
					union {
						dword_t virtual_address;
						dword_t reloc_count;             // Set to the real count when IMAGE_SCN_LNK_NRELOC_OVFL is set
					};
					dword_t symbol_table_index;
					word_t type;
				};
#include <ytl/detail/packed_2_end.hpp>




				std::size_t const sizeof_section_header		= 40;

				enum section_characteristics : dword_t
				{
//					scn_type_reg					= 0x00000000,		// Reserved.
//					scn_type_dsect					= 0x00000001,		// Reserved.
//					scn_type_noload					= 0x00000002,		// Reserved.
//					scn_type_group					= 0x00000004,		// Reserved.
					scn_type_no_pad					= 0x00000008,		// Reserved.
//					scn_type_copy					= 0x00000010,		// Reserved.

					scn_cnt_code					= 0x00000020,		// Section contains code.
					scn_cnt_initialized_data		= 0x00000040,		// Section contains initialized data.
					scn_cnt_uninitialized_data		= 0x00000080,		// Section contains uninitialized data.

					scn_lnk_other					= 0x00000100,		// Reserved.
					scn_lnk_info					= 0x00000200,		// Section contains comments or some other type of information.
//					scn_type_over					= 0x00000400,		// Reserved.
					scn_lnk_remove					= 0x00000800,		// Section contents will not become part of image.
					scn_lnk_comdat					= 0x00001000,		// Section contents comdat.
//													= 0x00002000,		// Reserved.
//					scn_mem_protected - Obsolete	= 0x00004000,		
					scn_no_defer_spec_exc			= 0x00004000,		// Reset speculative exceptions handling bits in the TLB entries for this section.
					scn_gprel						= 0x00008000,		// Section content can be accessed relative to GP
					scn_mem_fardata					= 0x00008000,
//					scn_mem_sysheap  - Obsolete		= 0x00010000,
					scn_mem_purgeable				= 0x00020000,
					scn_mem_16bit					= 0x00020000,
					scn_mem_locked					= 0x00040000,
					scn_mem_preload					= 0x00080000,

					scn_align_1bytes				= 0x00100000,		//
					scn_align_2bytes				= 0x00200000,		//
					scn_align_4bytes				= 0x00300000,		//
					scn_align_8bytes				= 0x00400000,		//
					scn_align_16bytes				= 0x00500000,		// Default alignment if no others are specified.
					scn_align_32bytes				= 0x00600000,		//
					scn_align_64bytes				= 0x00700000,		//
					scn_align_128bytes				= 0x00800000,		//
					scn_align_256bytes				= 0x00900000,		//
					scn_align_512bytes				= 0x00A00000,		//
					scn_align_1024bytes				= 0x00B00000,		//
					scn_align_2048bytes				= 0x00C00000,		//
					scn_align_4096bytes				= 0x00D00000,		//
					scn_align_8192bytes				= 0x00E00000,		//
// Unused											= 0x00F00000
					scn_align_mask					= 0x00F00000,

					scn_lnk_nreloc_ovfl				= 0x01000000,		// Section contains extended relocations.
					scn_mem_discardable				= 0x02000000,		// Section can be discarded.
					scn_mem_not_cached				= 0x04000000,		// Section is not cachable.
					scn_mem_not_paged				= 0x08000000,		// Section is not pageable.
					scn_mem_shared					= 0x10000000,		// Section is shareable.
					scn_mem_execute					= 0x20000000,		// Section is executable.
					scn_mem_read					= 0x40000000,		// Section is readable.
					scn_mem_write					= 0x80000000,		// Section is writeable.

//					TLS Chaacteristic Flags ?
					scn_scale_index					= 0x00000001		// Tls index is scaled
				};

				std::size_t const sizeof_short_name			= 8;

				struct section_header
				{
					byte_t name[sizeof_short_name];
					union {
						dword_t physical_address;
						dword_t virtual_size;
					} misc;

					dword_t virtual_address;
					dword_t size_of_raw_data;
					dword_t pointer_to_raw_data;
					dword_t pointer_to_relocations;
					dword_t pointer_to_linenumbers;
					word_t number_of_relocations;
					word_t number_of_linenumbers;

					dword_t characteristics;
				};




				enum symbol_section_number : short_t
				{
					sym_undefined		= (short_t)0,			// Symbol is undefined or is common.
					sym_absolute		= (short_t)-1,			// Symbol is an absolute value.
					sym_debug			= (short_t)-2,			// Symbol is a special debug item.
					sym_sectoin_max		= (short_t)0xfeff				// Values 0xFF00-0xFFFF are special
				};

#include <ytl/detail/packed_2_begin.hpp>
				struct symbol
				{
					union {
						byte_t short_name[8];
						struct {
							dword_t p_short;     // if 0, use LongName
							dword_t p_long;      // offset into string table
						} name;
						dword_t long_name[2];    // PBYTE [2]
					} N;
					dword_t value;
					short_t section_number;
					word_t type;
					byte_t storage_class;
					byte_t number_of_aux_symbols;
				};
#include <ytl/detail/packed_2_end.hpp>

			} // namespace image
		} // namspace coff
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_COFF_IMAGE_HPP*/