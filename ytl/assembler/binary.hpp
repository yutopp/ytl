#ifndef YTL_ASSEMBLER_BINARYHOLDER_HPP
#define YTL_ASSEMBLER_BINARYHOLDER_HPP

#include <vector>

#include <ytl/config.hpp>

#include "config.hpp"
#include "runnable_buffer_allocator.hpp"

#include "arch/detail/asm_status.hpp"

namespace ytl
{
	namespace assembler
	{
		template<template <typename T> class Allocator>
		class basic_raw_binary
		{
		public:
			typedef basic_raw_binary							this_type;

			typedef byte_t										value_type;
			typedef Allocator<value_type>						allocator_type;
			typedef std::vector<value_type, allocator_type>		wrapped_container_type;
			typedef typename wrapped_container_type::size_type	size_type;

		public:
			basic_raw_binary()
			{}
			
			basic_raw_binary( size_type const size )
				: binary_( size )
			{}

			template<typename IterT>
			basic_raw_binary( IterT const& begin, IterT const& end )
				: binary_( begin, end )
			{}

			template<template <typename T> class OtherA>
			basic_raw_binary( basic_raw_binary<OtherA> const & rhs )
				: binary_( rhs->cbegin(), rhs->cend() )
			{}

			wrapped_container_type& get()
			{
				return binary_;
			}

			wrapped_container_type const& get() const
			{
				return binary_;
			}

			wrapped_container_type& operator*()
			{
				return get();
			}

			wrapped_container_type const& operator*() const
			{
				return get();
			}

			wrapped_container_type* operator->()
			{
				return &get();
			}

			wrapped_container_type const* operator->() const
			{
				return &get();
			}

		private:
			wrapped_container_type binary_;
		};


		// ----------------------------------------------------------------------------------------------------


		// normal
		template<template <typename T> class Allocator>
		class basic_binary
			: public basic_raw_binary<Allocator>
		{
		public:
			typedef basic_binary								this_type;
			typedef basic_raw_binary<Allocator>					base_type;

			typedef typename base_type::value_type				value_type;
			typedef typename base_type::allocator_type			allocator_type;
			typedef typename base_type::wrapped_container_type	wrapped_container_type;
			typedef typename base_type::size_type				size_type;

			typedef detail::asm_status							asm_status_type;
			typedef std::shared_ptr<asm_status_type>			asm_status_pointer_type;
			typedef std::shared_ptr<asm_status_type const>		asm_status_const_pointer_type;

		public:
			basic_binary()
				: status_( new asm_status_type /*std::make_shared<asm_status_type>()*/ )
			{}
			
			basic_binary( size_type const size )
				: base_type( size )
			{}

			template<typename IterT>
			basic_binary( IterT const& begin, IterT const& end )
				: base_type( begin, end )
				, status_( new asm_status_type /*std::make_shared<asm_status_type>()*/ )
			{}

			template<template <typename T> class OtherA>
			basic_binary( basic_binary<OtherA> const & rhs )
				: base_type( rhs )
				, status_( new asm_status_type /*std::make_shared<asm_status_type>()*/ )
			{}

			asm_status_pointer_type get_status() { return status_; }
			asm_status_const_pointer_type get_status() const { return status_; }

		private:
			asm_status_pointer_type status_;
		};


		// runnable
		template<>
		class basic_binary<runnable_buffer_allocator>
			: public basic_raw_binary<runnable_buffer_allocator>
		{
		public:
			typedef basic_binary								this_type;
			typedef basic_raw_binary<runnable_buffer_allocator>	base_type;

			typedef base_type::value_type						value_type;
			typedef base_type::allocator_type					allocator_type;
			typedef base_type::wrapped_container_type			wrapped_container_type;
			typedef base_type::size_type						size_type;

			typedef detail::asm_status							asm_status_type;
			typedef std::shared_ptr<asm_status_type>			asm_status_pointer_type;
			typedef std::shared_ptr<asm_status_type const>		asm_status_const_pointer_type;

		public:
			basic_binary()
			{}
			
			basic_binary( size_type const size )
				: base_type( size )
			{}

			template<typename IterT>
			basic_binary( IterT const& begin, IterT const& end )
				: base_type( begin, end )
			{}

			template<template <typename T> class OtherA>
			basic_binary( basic_binary<OtherA> const & rhs )
				: base_type( rhs )
			{}

			template<typename ReturnT>
			ReturnT cast_call( /*prms*/ ) const
			{
				typedef ReturnT(*function_type)( /*prms*/ );
				return (*reinterpret_cast<function_type>( &(*this)->data()[0] ))( /*args*/ );
			}

			asm_status_pointer_type get_status() { return status_; }
			asm_status_const_pointer_type get_status() const { return status_; }

		private:
			asm_status_pointer_type status_;
		};


		// ----------------------------------------------------------------------------------------------------


		template<template <typename> class T, template <typename> class U>
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
		}


		// ----------------------------------------------------------------------------------------------------


		// default binary holder
		typedef basic_binary<std::allocator> binary;

		// runnable binary holder
		typedef basic_binary<runnable_buffer_allocator> runnable_binary;


	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_BINARYHOLDER_HPP*/