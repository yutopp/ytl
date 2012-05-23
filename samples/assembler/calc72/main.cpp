#include <ytl/assembler.hpp>

#include <iostream>

int main()
{
	namespace x86 = ytl::assembler::ia_32;
	using namespace x86::registers;
	using namespace x86::alignment;

	ytl::assembler::code_stub code( 2048 );
	auto as = ytl::assembler::make_generator<x86::engine>( code );

	as()
		.push	( ecx )			//test
		.sub	( esp, 80 )		//

		.mov	( ebx, esp )
		.mov	( esp, ebx )

		.mov	( edx, 100 )

		.sub	( edx, 17 )
		.sub	( dl, 1 )

		.add	( ecx, 4 )

		.add	( al, 4 )		//
//		.add	( ax, 4 );
		.add	( eax, 4 )		//
		.mov	( ptr[ebx], edx )
		.add	( ptr[ebx], -5 )
		.mov	( ecx, ptr[ebx] )

		.mov	( eax, ebx )
		.mov	( edx, 2 )
		.mov	( ptr[eax + edx * 2 + 4], ecx )
		.sub	( ptr[eax + edx * 2 + 4], 5 )
		.mov	( ecx, ptr[eax + edx * 2 + 4] )

		.mov	( ptr[ebx+4], ecx )
		.mov	( eax, ptr[ebx+4] )

		.sub	( esp, -80 )
		.pop	( ecx )
		.ret	();


	typedef int(*func_t)();

	// Call genarated function code!
	int const i = (reinterpret_cast<func_t>( &code[0] ))();

	std::cout<< "result: " << i << std::endl;

	{ char c; std::cin >> c; }
}

