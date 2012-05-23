#ifndef YTL_ASSEMBLER_CODE_STUB_HPP
#define YTL_ASSEMBLER_CODE_STUB_HPP

#include <vector>

#include <ytl/config/platform.hpp>

#include "config.hpp"

#ifdef YTL_WINDOWS
# include "detail/windows_code_stub.hpp"
#endif

namespace ytl
{
	namespace assembler
	{
#ifdef YTL_WINDOWS
		namespace detail { typedef windows_runnable_buffer_allocator<byte_t> runnable_buffer_allocator; }
#endif

		typedef std::vector<byte_t, detail::runnable_buffer_allocator> code_stub;

	} // namespace assembeler
} // namespace ytl

#endif /*YTL_ASSEMBLER_CODE_STUB_HPP*/