#ifndef YTL_ASSEMBLER_ARCH_UTILITY_HPP
#define YTL_ASSEMBLER_ARCH_UTILITY_HPP

#define YTL_ASM_OP_BEGIN(decl) return_type& decl {
#define YTL_ASM_OP_END return static_cast<return_type&>( *this ); }

#endif /*YTL_ASSEMBLER_ARCH_UTILITY_HPP*/