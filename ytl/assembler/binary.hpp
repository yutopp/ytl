#ifndef YTL_ASSEMBLER_BINARYHOLDER_HPP
#define YTL_ASSEMBLER_BINARYHOLDER_HPP

#include <utility>

#include <ytl/buffer/binary_buffer.hpp>

#include "config.hpp"
#include "detail/status_holder.hpp"

namespace ytl
{
	namespace assembler
	{
		template<template <typename T> class Allocator>
		class code_buffer
		{
		public:
			typedef code_buffer									self_type;

			typedef binary_buffer<Allocator>					container_type;
			typedef std::shared_ptr<container_type>				container_shared_pointer;
			typedef std::shared_ptr<container_type const>		const_container_shared_pointer;

		public:
/*			basic_binary()
				: status_( new asm_status_type )
			{}
			
*/
			container_shared_pointer get_container()
			{
				return container_;
			}

			const_container_shared_pointer get_container() const
			{
				return container_;
			}


			detail::status_holder_ptr get_status()
			{
				return status_;
			}

			detail::const_status_holder_ptr get_status() const
			{
				return status_;
			}


			void swap( self_type& rhs )
			{
				using std::swap;

				swap( buffer_, rhs.buffer_ );
				swap( status_, rhs.status_ );
			}

		private:
			container_shared_pointer container_;
			detail::status_holder_ptr status_;
		};




		// ----------------------------------------------------------------------------------------------------


/*		template<template <typename> class T, template <typename> class U>
		inline basic_raw_binary<T>& operator<<( basic_raw_binary<T>& lhs, basic_raw_binary<U> const& rhs )
		{
			lhs->insert( lhs->end(), rhs->cbegin(), rhs->cend() );
			return lhs;
		}

		template<template <typename> class T, template <typename> class U, std::size_t N>
		inline basic_raw_binary<T>& operator<<( basic_raw_binary<T>& lhs, typename basic_raw_binary<U>::value_type const (&rhs)[N] )
		{
			lhs->insert( lhs->end(), rhs, rhs + N );
			return lhs;
		}

		template<template <typename> class T>
		inline std::ostream& operator<<( std::ostream& os, basic_raw_binary<T> const& buffer )
		{
			os.write( reinterpret_cast<char const*>( &buffer->data()[0] ), buffer->size() );
			return os;
		}*/


		// ----------------------------------------------------------------------------------------------------


		// default binary holder
//		typedef basic_binary<std::allocator> binary;

		// runnable binary holder
//		typedef basic_binary<runnable_buffer_allocator> runnable_binary;


	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_BINARYHOLDER_HPP*/