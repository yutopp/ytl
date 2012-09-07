// PUSH
YTL_ASM_OP_LAZY_1( push )

YTL_ASM_OP_BEGIN( push( r_m16 src ) )
	this->db( 0xff );
	dmod_rm_sib_disp( src, 6 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( r_m32 src ) )
	this->db( 0xff );
	dmod_rm_sib_disp( src, 6 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::r16_t reg ) )
	this->db( 0x50 + reg );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::r32_t reg ) )
	this->db( 0x50 + reg );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( byte_value value ) )
	this->db( 0x6a );
	this->db( value );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( word_value value ) )
	this->db( 0x68 );
	this->dw( value );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( dword_value value ) )
	this->db( 0x68 );
	this->dd( value );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::cs_t ) )
	this->db( 0x0e );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::ss_t ) )
	this->db( 0x16 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::ds_t ) )
	this->db( 0x1e );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::es_t ) )
	this->db( 0x06 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::fs_t ) )
	this->db( 0x0f );
	this->db( 0xa0 );
YTL_ASM_OP_END

YTL_ASM_OP_BEGIN( push( registers::gs_t ) )
	this->db( 0x0f );
	this->db( 0xa8 );
YTL_ASM_OP_END



// PUSHA
YTL_ASM_OP_BEGIN( pusha() )
	this->db( 0x60 );
YTL_ASM_OP_END



// PUSHAD
YTL_ASM_OP_BEGIN( pushad() )
	this->db( 0x60 );
YTL_ASM_OP_END



// PUSHF
YTL_ASM_OP_BEGIN( pushf() )
	this->db( 0x9c );
YTL_ASM_OP_END



// PUSHFD
YTL_ASM_OP_BEGIN( pushfd() )
	this->db( 0x9c );
YTL_ASM_OP_END