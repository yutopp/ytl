#ifndef YTL_DETAIL_BUFFER_BASE_HPP
#define YTL_DETAIL_BUFFER_BASE_HPP

#include <cstddef>

#include <type_traits>

#include <ytl/config/typedef.hpp>

namespace ytl
{
	namespace detail
	{
		//
		struct binary_buffer_policy
		{
			typedef byte_t							value_type;
			typedef value_type&						reference;
			typedef value_type const&				const_reference;
			typedef value_type*						pointer;
			typedef value_type const*				const_pointer;

			typedef std::size_t						size_type;
			typedef std::ptrdiff_t					difference_type;
		};


		//
		template<
			template<typename Value, template<typename> class Allocator> class ContainerTraits
		>
		struct data_member_traits
		{
			template<typename T>
			static auto data( T& container )
				-> decltype( container.data() )
			{
				return container.data();
			}

			template<typename T>
			static auto data( T const& container )
				-> decltype( container.data() )
			{
				return container.data();
			}
		};


		//
		template<typename Container>
		struct container_copy_traits
		{
			typedef Container			container_type;

			template<typename Iter>
			static Container copy( Container const&, Iter const& begin, Iter const& end )
			{
				return Container( begin, end );
			}
		};



		//
		template<typename Container, typename MemberTraits>
		struct pointer_type_traits
		{
			typedef decltype( MemberTraits::data( std::declval<Container>() ) )			pointer;
			typedef decltype( MemberTraits::data( std::declval<Container const>() ) )	const_pointer;
		};


		//
		template<typename Unused>
		struct unused_allocator {};


		//		
		template<
			typename Derived,
			template<typename Value> class Allocator,
			template<typename Value, template<typename> class Allocator> class ContainerPolicy
		>
		class fixed_buffer_base
		{
		public:
			typedef fixed_buffer_base													base_type;
			typedef Derived																derived_type;

			typedef byte_t																value_type;
			typedef typename ContainerPolicy<value_type, Allocator>::type				wrapped_container_type;
			typedef Allocator<value_type>												allocator_type;
			
		private:
			typedef data_member_traits<ContainerPolicy>									data_function_traits;
			typedef pointer_type_traits<wrapped_container_type, data_function_traits>	data_pointer_type_traits;

		public:
			typedef typename wrapped_container_type::iterator							iterator;
			typedef typename wrapped_container_type::const_iterator						const_iterator;
			typedef typename wrapped_container_type::reference							reference;
			typedef typename wrapped_container_type::difference_type					difference_type;
			typedef typename wrapped_container_type::size_type							size_type;

			typedef typename data_pointer_type_traits::pointer							pointer;
			typedef typename data_pointer_type_traits::const_pointer					const_pointer;

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
			inline pointer data()
			{
				return data_function_traits::data( *(*this) );
			}

			inline const_pointer data() const
			{
				return data_function_traits::data( *(*this) );
			}

			inline value_type& at( size_type const index )
			{
				return (*this)->at( index );
			}

			inline value_type const& at( size_type const index ) const
			{
				return (*this)->at( index );
			}

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
			typename Derived,
			template<typename Value> class Allocator,
			template<typename Value, template<typename> class Allocator> class ContainerPolicy
		>
		class resizable_buffer_base
			: public fixed_buffer_base<Derived, Allocator, ContainerPolicy>
		{
		public:
			typedef resizable_buffer_base							self_type;

			typedef typename self_type::value_type					value_type;
			typedef typename self_type::size_type					size_type;

		public:
			inline void resize( size_type const size )
			{
				(*this)->resize( size );
			}

			inline void push_back( value_type const& value )
			{
				(*this)->push_back( value );
			}
		};


		// ----------------------------------------------------------------------------------------------------
		template<
			typename LhsDerived,
			template<typename Value> class LhsAllocator,
			template<typename Value, template<typename> class Allocator> class LhsContainerPolicy,
			typename RhsDerived,
			template<typename Value> class RhsAllocator,
			template<typename Value, template<typename> class Allocator> class RhsContainerPolicy
		>
		inline typename fixed_buffer_base<LhsDerived, LhsAllocator, LhsContainerPolicy>::derived_type&
			operator<<(
				fixed_buffer_base<LhsDerived, LhsAllocator, LhsContainerPolicy>& lhs,
				fixed_buffer_base<RhsDerived, RhsAllocator, RhsContainerPolicy> const& rhs
				)
		{
			lhs->insert( lhs->end(), rhs->begin(), rhs->end() );

			return static_cast<
						typename fixed_buffer_base<LhsDerived, LhsAllocator, LhsContainerPolicy>::derived_type&
					>( lhs );
		}

		template<
			typename LhsDerived,
			template<typename Value> class LhsAllocator,
			template<typename Value, template<typename> class Allocator> class LhsContainerPolicy,
			std::size_t N
		>
		inline typename fixed_buffer_base<LhsDerived, LhsAllocator, LhsContainerPolicy>::derived_type&
			operator<<(
				fixed_buffer_base<LhsDerived, LhsAllocator, LhsContainerPolicy>& lhs,
				typename fixed_buffer_base<LhsDerived, LhsAllocator, LhsContainerPolicy>::value_type const (&rhs)[N]
				)
		{
			lhs->insert( lhs->end(), rhs, rhs + N );

			return static_cast<
						typename fixed_buffer_base<LhsDerived, LhsAllocator, LhsContainerPolicy>::derived_type&
					>( lhs );
		}

		template<
			typename Derived,
			template<typename Value> class Allocator,
			template<typename Value, template<typename> class Allocator> class ContainerPolicy,
			std::size_t N
		>
		inline std::ostream& operator<<(
			std::ostream& os,
			typename fixed_buffer_base<Derived, Allocator, ContainerPolicy>::derived_type const& buffer
			)
		{
			os.write( reinterpret_cast<char const*>( &buffer->data()[0] ), buffer->size() );

			return os;
		}
		// ----------------------------------------------------------------------------------------------------

	} // namespace detail
} // namespace ytl

#endif /*YTL_DETAIL_BUFFER_BASE_HPP*/