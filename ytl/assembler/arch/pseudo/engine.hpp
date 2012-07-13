#ifndef YTL_ASSEMBLER_PSEUDO_ENGINE_HPP
#define YTL_ASSEMBLER_PSEUDO_ENGINE_HPP

#include "../../config.hpp"
#include "../../exception.hpp"

#include "../detail/pseudo_operations.hpp"


namespace ytl
{
	namespace assembler
	{
		namespace pseudo
		{
			template<typename Writer>
			class engine
				: public detail::pseudo_operations<engine, Writer>
			{
				typedef engine			self_type;

				typedef Writer			writer_type;
				typedef std::size_t		size_type;

				typedef self_type		return_type;

			public:
				engine( writer_type& w )
					: pseudo_operations( w )
				{}
			};

		} // namespace pseudo
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_PSEUDO_ENGINE_HPP*/