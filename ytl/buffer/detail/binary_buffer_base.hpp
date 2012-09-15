#ifndef YTL_DETAIL_BUFFER_BASE_HPP
#define YTL_DETAIL_BUFFER_BASE_HPP

#include <type_traits>

#include <ytl/config/typedef.hpp>

namespace ytl
{
	namespace detail
	{
		template<
			template<template<typename Value> class Allocator> class Derived,
			template<typename Value> class Allocator,
			template<typename Value, typename Allocator> class ContainerTraits
		>
		class fixed_buffer_base
		{
		public:
			typedef fixed_buffer_base												base_type;

			typedef byte_t															value_type;
			typedef Allocator<value_type>											allocator_type;

		private:
			typedef Derived<Allocator>												derived_type;

		public:
			typedef typename ContainerTraits<value_type, allocator_type>::type		wrapped_container_type;

			typedef typename wrapped_container_type::iterator						iterator;
			typedef typename wrapped_container_type::const_iterator					const_iterator;
			typedef typename wrapped_container_type::reference						reference;
			typedef typename wrapped_container_type::const_reference				const_reference;
			typedef typename wrapped_container_type::pointer						pointer;
			typedef typename wrapped_container_type::difference_type				difference_type;
			typedef typename wrapped_container_type::size_type						size_type;

		public:
			// accessor
			inline wrapped_container_type& operator*()
			{
				return reinterpret_cast<derived_type&>( *this ).get();
			}

			inline wrapped_container_type const& operator*() const
			{
				return reinterpret_cast<derived_type const&>( *this ).get();
			}

			inline wrapped_container_type* operator->()
			{
				return &*(*this);
			}

			inline wrapped_container_type const* operator->() const
			{
				return &*(*this);
			}

			// Member function interfaces of container

			inline iterator begin()
			{
				return (*this)->begin();
			}

			inline const_iterator begin() const
			{
				return (*this)->begin();
			}

			inline iterator end()
			{
				return (*this)->end();
			}

			inline const_iterator end() const
			{
				return (*this)->end();
			}

			inline size_type size() const
			{
				return (*this)->size();
			}

			inline size_type max_size() const
			{
				return (*this)->max_size();
			}

			inline bool empty() const
			{
				return (*this)->empty();
			}


			// Requires

			inline value_type& operator[]( size_type const index )
			{
				return (*this)->operator[]( index );
			}

			inline value_type const& operator[]( size_type const index ) const
			{
				return (*this)->operator[]( index );
			}
		};



		template<
			template<template<typename Value> class Allocator> class Derived,
			template<typename Value> class Allocator,
			template<typename Value, typename Allocator> class ContainerTraits
		>
		class resizable_buffer_base
			: public fixed_buffer_base<Derived, Allocator, ContainerTraits>
		{
		public:
			typedef resizable_buffer_base							self_type;
			typedef typename self_type::base_type					base_type;

			typedef typename base_type::value_type					value_type;
			typedef typename base_type::size_type					size_type;

		public:

			// Requires

			inline void resize()( size_type const size )
			{
				(*this)->resize( size );
			}

			inline void push_back( value_type const& value )
			{
				(*this)->push_back( value );
			}
		};

	} // namespace detail
} // namespace ytl

#endif /*YTL_DETAIL_BUFFER_BASE_HPP*/