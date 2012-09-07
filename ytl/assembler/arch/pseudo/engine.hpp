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
				: public detail::pseudo_operations<engine<Writer>, Writer>
			{
				typedef engine											self_type;
				typedef detail::pseudo_operations<self_type, Writer>	base_type;

			public:
				typedef typename self_type::writer_type					writer_type;
				typedef typename self_type::writer_shared_pointer		writer_shared_pointer;

				typedef typename self_type::status_type					status_type;
				typedef typename self_type::status_shared_pointer		status_shared_pointer;

				typedef typename self_type::size_type					size_type;
				typedef typename self_type::return_type					return_type;

			public:
				engine( writer_shared_pointer w, status_shared_pointer s )
					: base_type( w, s )
				{}
			};

		} // namespace pseudo
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_PSEUDO_ENGINE_HPP*/