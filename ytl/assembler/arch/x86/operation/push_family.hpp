// PUSH
YTL_ASM_OP_LAZY_1( push )

YTL_ASM_OP_BEGIN( push( r_m16 src ) )
	db( 0xff );
	dmod_rm_sib_disp( src, 6 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( r_m32 src ) )
	db( 0xff );
	dmod_rm_sib_disp( src, 6 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::r16_t reg ) )
	db( 0x50 + reg );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::r32_t reg ) )
	db( 0x50 + reg );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( byte_value value ) )
	db( 0x6a );
	db( value );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( word_value value ) )
	db( 0x68 );
	dw( value );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( dword_value value ) )
	db( 0x68 );
	dd( value );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::cs_t ) )
	db( 0x0e );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::ss_t ) )
	db( 0x16 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::ds_t ) )
	db( 0x1e );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::es_t ) )
	db( 0x06 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::fs_t ) )
	db( 0x0f );
	db( 0xa0 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::gs_t ) )
	db( 0x0f );
	db( 0xa8 );
YTL_ASM_OP_END



// PUSHA
YTL_ASM_OP_BEGIN( pusha() )
	db( 0x60 );
YTL_ASM_OP_END



// PUSHAD
YTL_ASM_OP_BEGIN( pushad() )
	db( 0x60 );
YTL_ASM_OP_END



// PUSHF
YTL_ASM_OP_BEGIN( pushf() )
	db( 0x9c );
YTL_ASM_OP_END



// PUSHFD
YTL_ASM_OP_BEGIN( pushfd() )
	db( 0x9c );
YTL_ASM_OP_END