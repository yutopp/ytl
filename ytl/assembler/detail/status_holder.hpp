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

			typedef std::shared_ptr<status_holder>			status_holder_shared_ptr;
			typedef std::shared_ptr<status_holder const>	const_status_holder_shared_ptr;

			template<typename Buffer>
			struct get_asm_status_traits
			{
				static status_holder_shared_ptr get( Buffer& )
				{
					status_holder_shared_ptr p( new status_holder );

					return p;
				}
			};

		} // namespace detail
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_DETAIL_STATUS_HOLDER_HPP*/