#ifndef YTL_ASSEMBLER_DETAIL_STATUS_HOLDER_HPP
#define YTL_ASSEMBLER_DETAIL_STATUS_HOLDER_HPP

#include <memory>

namespace ytl
{
	namespace assembler
	{
		namespace detail
		{
			class status_holder
			{
			};

			typedef std::shared_ptr<status_holder>			status_holder_ptr;
			typedef std::shared_ptr<status_holder const>	const_status_holder_ptr;

		} // namespace detail
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_DETAIL_STATUS_HOLDER_HPP*/