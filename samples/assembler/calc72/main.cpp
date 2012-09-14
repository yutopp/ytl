#include <ytl/assembler.hpp>

#include <iostream>

int main()
{
	namespace yasm = ytl::assembler;
	namespace x86 = yasm::x86;
	using namespace x86::registers;
	using namespace x86::alignment;

	yasm::runnable_binary<int>code( 2048 );
	ytl::assembler::generator<x86::engine> as;

	as( code )
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


	// Call genarated function code!
	int const i = code.call<int>();

	std::cout<< "result: " << i << std::endl;

	{ char c; std::cin >> c; }
}

