#ifndef YTL_ASSEMBLER_BINARY_CODE_HPP
#define YTL_ASSEMBLER_BINARY_CODE_HPP

#include <utility>

#include <ytl/buffer/runnable_buffer_allocator.hpp>
#include <ytl/buffer/binary_buffer.hpp>

#include "config.hpp"
#include "detail/status_holder.hpp"

namespace ytl
{
	namespace assembler
	{
		template<template <typename> class Allocator>
		class basic_binary_code;	// forward declaration

		namespace detail
		{
			using namespace ytl::detail;

			template<typename Value, template<typename> class Allocator>
			struct binary_code_container
			{
				typedef basic_binary_buffer<Allocator>		type;
			};

            //
			template<template <typename> class Allocator>
			struct get_asm_status_traits<basic_binary_code<Allocator>>
			{
				static status_holder_shared_ptr get( basic_binary_code<Allocator>& buffer )
				{
					return buffer.get_status();
				}
			};
		} // namespace detail


		// binary code
        // difference between basic_binary_code and binary_buffer_basic
        //  - basic_binary_code has status for assembler generator(ex, labels)
		template<template <typename> class Allocator>
		class basic_binary_code
			: public detail::resizable_buffer_base<
						basic_binary_code<Allocator>, Allocator,
						detail::binary_code_container
					>
		{
		public:
			typedef basic_binary_code								self_type;
			typedef typename self_type::wrapped_container_type		wrapped_container_type;

			typedef detail::status_holder							status_type;
			typedef detail::status_holder_shared_ptr				status_shared_pointer;
			typedef detail::const_status_holder_shared_ptr			const_status_shared_pointer;

		public:
			wrapped_container_type& get()
			{
				return container_;
			}

			wrapped_container_type const& get() const
			{
				return container_;
			}


			status_shared_pointer get_status()
			{
				return status_;
			}

			const_status_shared_pointer get_status() const
			{
				return status_;
			}

			void swap( self_type& rhs )
			{
				using std::swap;

				swap( container_, rhs.container_ );
				swap( status_, rhs.status_ );
			}

		private:
			wrapped_container_type container_;
			status_shared_pointer status_;
		};

		// default binary holder
		typedef basic_binary_code<std::allocator> binary_code;

		// runnable binary holder
		typedef basic_binary_code<runnable_buffer_allocator> runnable_binary_code;

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_BINARY_CODE_HPP*/