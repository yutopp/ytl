// POP
/*void push( m16 src )
{
	db( 0xff );
	dmod_rm_sib_disp( src, 6 );
}

void push( m32 src )
{
	db( 0xff );
	dmod_rm_sib_disp( src, 6 );
}*/

YTL_ASM_OP_BEGIN( pop( registers::r16_t reg ) )
	db( 0x58 + reg );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( pop( registers::r32_t reg ) )
	db( 0x58 + reg );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( pop( registers::ds_t ) )
	db( 0x1f );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( pop( registers::es_t ) )
	db( 0x07 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( pop( registers::ss_t ) )
	db( 0x17 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( pop( registers::fs_t ) )
	db( 0x0f );
	db( 0xa1 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( pop( registers::gs_t ) )
	db( 0x0f );
	db( 0xa9 );
YTL_ASM_OP_END



// POPA
YTL_ASM_OP_BEGIN( popa() )
	db( 0x61 );
YTL_ASM_OP_END



// POPAD
YTL_ASM_OP_BEGIN( popad() )
	db( 0x61 );
YTL_ASM_OP_END



// POPF
YTL_ASM_OP_BEGIN( popf() )
	db( 0x9d );
YTL_ASM_OP_END



// POPFD
YTL_ASM_OP_BEGIN( popfd() )
	db( 0x9d );
YTL_ASM_OP_END