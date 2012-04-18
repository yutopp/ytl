#include <ytl/assembler.hpp>

#include <iostream>

int main() {
	namespace x86 = ytd::assembler::ia_32;
	using namespace x86::registers;
	using namespace x86::alignment;

	ytd::assembler::code_stub code( 2048 );
	x86::generator<ytd::assembler::code_stub> gen( code );
	gen.push( ecx );		//test

	gen.sub( esp, 80 );		//

	gen.mov( ebx, esp );
	gen.mov( esp, ebx );

	gen.mov( edx, 100 );

	gen.sub( edx, 17 );
	gen.sub( dl, 1 );

	gen.add( ecx, 4 );

	gen.add( al, 4 );	//
//	gen.add( ax, 4 );
	gen.add( eax, 4 );	//
	gen.mov( ptr[ebx], edx );
	gen.add( ptr[ebx], -5 );
	gen.mov( ecx, ptr[ebx] );

	gen.mov( eax, ebx );
	gen.mov( edx, 2 );
	gen.mov( ptr[eax + edx * 2 + 4], ecx );
	gen.sub( ptr[eax + edx * 2 + 4], 5 );
	gen.mov( ecx, ptr[eax + edx * 2 + 4] );

	gen.mov( ptr[ebx+4], ecx );
	gen.mov( eax, ptr[ebx+4] );

	gen.sub( esp, -80 );
	gen.pop( ecx );
	gen.ret();


	typedef int(*func_t)();

	int const i = (reinterpret_cast<func_t>( &code[0] ))();
	std::cout<< "tihaya " << i << std::endl;

	{ char c; std::cin >> c; }
}

