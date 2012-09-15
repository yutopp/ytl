#ifndef YTL_ASSEMBLER_DETAIL_BUFFER_WRAPPER_HPP
#define YTL_ASSEMBLER_DETAIL_BUFFER_WRAPPER_HPP

#include <cstddef>
#include <utility>
#include <memory>

namespace ytl
{
	namespace assembler
	{
		namespace detail
		{
			// default wraapper implemantation.
			template<typename Buffer>
			struct buffer_wrapper
			{
				typedef Buffer								buffer_type;
				typedef typename buffer_type::value_type	value_type;
				typedef std::size_t							index_type;
				typedef std::shared_ptr<index_type>			index_pointer_type;
				typedef std::shared_ptr<index_type const>	index_const_pointer_type;

			public:
				buffer_wrapper( buffer_type& b, index_type const index = 0u )
					: buffer_( b )
					, index_( std::make_shared<index_type>( index ) )
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

			public:
				buffer_type& buffer_;
				index_pointer_type index_;
			};

			// fixed size.
			template<typename T, std::size_t N>
			struct buffer_wrapper<T[N]>
			{
				typedef T*									buffer_type;
				typedef T									value_type;
				typedef std::size_t							index_type;
				typedef std::shared_ptr<index_type>			index_pointer_type;
				typedef std::shared_ptr<index_type const>	index_const_pointer_type;

			public:
				buffer_wrapper( buffer_type b, index_type const index = 0u )
					: buffer_( b )
					, index_( std::make_shared<index_type>( index ) )
				{}

				std::size_t size() const
				{
					return N;
				}

				value_type* data()
				{
					return &buffer_[0];
				}

	//			void resize( std::size_t ) {}

			public:
				buffer_type buffer_;	// pointer
				index_pointer_type index_;
			};
		} // namespace detail
	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_DETAIL_BUFFER_WRAPPER_HPP*/