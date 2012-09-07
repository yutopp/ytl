#ifndef YTL_ASSEMBLER_RUNNABLE_BUFFER_ALLOCATOR_HPP
#define YTL_ASSEMBLER_RUNNABLE_BUFFER_ALLOCATOR_HPP

#include "config.hpp"

#ifdef YTL_WINDOWS
# include "detail/windows_runnable_buffer_allocator.hpp"
#else
# error Not supported...
#endif

namespace ytl
{
	namespace assembler
	{
		template<typename T>
		class runnable_buffer_allocator 
#ifdef YTL_WINDOWS
			: public windows::runnable_buffer_allocator<T>
#else
# error Not supported...
#endif
		{};

	} // namespace assembeler
} // namespace ytl

#endif /*YTL_ASSEMBLER_RUNNABLE_BUFFER_ALLOCATOR_HPP*/