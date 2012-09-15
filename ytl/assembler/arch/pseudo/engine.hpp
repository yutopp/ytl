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
				typedef detail::pseudo_operations<engine, Writer>	base_type;

			public:
				typedef typename base_type::writer_type				writer_type;
				typedef typename base_type::writer_pointer_type		writer_pointer_type;

				typedef typename base_type::size_type				size_type;
				typedef typename base_type::return_type				return_type;

			public:
				engine( writer_pointer_type w, detail::status_holder_ptr s )
					: base_type( w, s )
				{}
			};

		} // namespace pseudo
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_PSEUDO_ENGINE_HPP*/