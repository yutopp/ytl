#ifndef YTL_ASSEMBLER_DETAIL_BUFFER_WRAPPER_HPP
#define YTL_ASSEMBLER_DETAIL_BUFFER_WRAPPER_HPP

#include <cstddef>
#include <utility>

namespace ytl
{
	namespace assembler
	{
		/* --------
			template<typename T>
			class buffer_wrapper

			[Comments]
			This is used by (fixed | variable)_writer.

			[Require]
			public:
				typedef **** value_type;	: element's type of buffer.
				typedef **** index_type;	: type of index.
				size_type size() const		: return size of buffer.
				value_type* data()			: return head address of buffer.
			(for variable_writer)
				void resize( size_type )	: resize buffer. it's required only when using variavle_writer.
		-------- */

		// default wraapper implemantation.
		template<typename Buffer>
		struct buffer_wrapper
		{
			typedef Buffer								buffer_type;
			typedef typename buffer_type::value_type	value_type;
			typedef std::size_t							index_type;

			buffer_wrapper( buffer_type& b, index_type const index = 0u )
				: index_( index )
				, buffer_( b )
			{}

			std::size_t size() const
			{
				return buffer_.size();
			}

			value_type* data()
			{
				return &buffer_[0];
			}

			void resize( std::size_t const size )
			{
				buffer_.resize( size );
			}

			index_type index_;
			buffer_type& buffer_;
		};

		// fixed size.
		template<typename T, std::size_t N>
		struct buffer_wrapper<T[N]>
		{
			typedef T*				buffer_type;
			typedef T				value_type;
			typedef std::size_t		index_type;

			buffer_wrapper( buffer_type b, index_type const index = 0u )
				: index_( index )
				, buffer_( b )
			{}

			std::size_t size() const
			{
				return N;
			}

			value_type* data()
			{
				return &buffer_[0];
			}

			index_type index_;
			buffer_type buffer_;	// pointer
		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_DETAIL_BUFFER_WRAPPER_HPP*/