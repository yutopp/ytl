#ifndef YTL_FILEFORMAT_DETAIL_VALID_BUFFER_HOLDER_HPP
#define YTL_FILEFORMAT_DETAIL_VALID_BUFFER_HOLDER_HPP

#include <memory>
#include <type_traits>

#include <ytl/buffer/detail/binary_buffer_base.hpp>
#include <ytl/utility/guard_macros.hpp>

namespace ytl
{
	namespace fileformat
	{
		namespace detail
		{
			struct unused_dummy_validator
			{
				template<typename T>
				void operator()( T const& ) const {}
			};


			template<typename Validator = unused_dummy_validator>
			class immutable_valid_buffer_holder
			{
			public:
				typedef ytl::detail::binary_buffer_policy::value_type			value_type;
				typedef ytl::detail::binary_buffer_policy::const_pointer		const_pointer;
				typedef ytl::detail::binary_buffer_policy::size_type			size_type;

			private:
				// 
				class holder_base
				{
				public:
					virtual ~holder_base() {}

					virtual const_pointer data() const =0;
					virtual const_pointer begin() const =0;
					virtual const_pointer end() const =0;
					virtual size_type size() const =0;
				};

				//
				template<typename Buffer, typename ValidatorType>
				class holder
					: public holder_base
				{
					YTL_REQUIRE_BINARY_BUFFER( Buffer )

					typedef typename std::remove_reference<Buffer>::type	buffer_type;

				public:
					holder( buffer_type&& buffer )
						: buffer_( std::move( buffer ) )
					{
						ValidatorType()( buffer_ );
					}

					const_pointer data() const
					{
						return buffer_.data();
					}

					const_pointer begin() const
					{
						return buffer_.begin();
					}

					const_pointer end() const
					{
						return buffer_.end();
					}

					size_type size() const
					{
						return buffer_.size();
					}

				public:
					buffer_type buffer_;
				};

			private:
				typedef std::shared_ptr<holder_base>		holder_type;

			public:
				// for rvalue
				template<typename Buffer>
				immutable_valid_buffer_holder(
					Buffer&& buffer,
					typename std::enable_if<std::is_rvalue_reference<Buffer&&>::value>::type* =0
					)
					: holder_( std::make_shared<holder<Buffer, Validator>>( std::move( buffer ) ) )
				{}

				// for lvalue(do copy)
				template<typename Buffer>
				immutable_valid_buffer_holder(
					Buffer&& buffer,
					typename std::enable_if<!std::is_rvalue_reference<Buffer&&>::value>::type* =0
					)
					: holder_( std::make_shared<
										holder<
											typename std::remove_const<typename std::remove_reference<Buffer>::type>::type,
											Validator
											>
										>( typename std::remove_const<typename std::remove_reference<Buffer>::type>::type( buffer ) ) )
				{}

				// unique_ptr
				template<typename Buffer>
				immutable_valid_buffer_holder(
					std::unique_ptr<Buffer> buffer/*,
					typename std::enable_if<!std::is_reference<Buffer>::value>::type* =0		// only value contain pointer */
					)
					: holder_( std::make_shared<holder<Buffer, Validator>>( std::move( *buffer ) ) )
				{}


				const_pointer data() const
				{
					return holder_->data();
				}

				const_pointer begin() const
				{
					return holder_->begin();
				}

				const_pointer end() const
				{
					return holder_->end();
				}

				size_type size() const
				{
					return holder_->size();
				}

			private:
				holder_type holder_;
			};



			template<typename Buffer, typename Validator = unused_dummy_validator>
			class buffer_holder
			{
			public:
				typedef Buffer											buffer_type;
				typedef typename std::add_const<buffer_type>::type		const_buffer_type;
				typedef std::shared_ptr<buffer_type>					buffer_shared_pointer;

			public:
				buffer_holder( buffer_shared_pointer buffer )
					: pointer_( buffer )
				{
					Validator()( pointer_ );
				}

				buffer_type& operator*() { return *pointer_; }
				buffer_type& operator->() { return *pointer_; }

				const_buffer_type const& operator*() const { return *pointer_; }
				const_buffer_type const& operator->() const { return *pointer_; }

				buffer_shared_pointer get_pointer() const
				{
					return pointer_;
				}

			private:
				buffer_shared_pointer pointer_;
			};

		} // namespace detail
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_DETAIL_VALID_BUFFER_HOLDER_HPP*/