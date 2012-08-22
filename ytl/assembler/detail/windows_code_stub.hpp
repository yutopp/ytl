#ifndef YTL_ASSEMBLER_WINDOWS_RUNNABLE_BUFFER_ALLOCATOR_HPP
#define YTL_ASSEMBLER_WINDOWS_RUNNABLE_BUFFER_ALLOCATOR_HPP

#include <cstddef>
#include <cstring>
#include <exception>

#include <ytl/config/windows.hpp>

#include "../config.hpp"

namespace ytl
{
	namespace assembler
	{
		namespace windows
		{
			template<typename T>
			class runnable_buffer_allocator
			{
			public:
				typedef std::size_t size_type;
				typedef std::ptrdiff_t difference_type;
				typedef T* pointer;
				typedef T const* const_pointer;
				typedef T& reference;
				typedef T const& const_reference;
				typedef T value_type;

				template<typename U>
				struct rebind
				{
					typedef runnable_buffer_allocator<U> other;
				};
	
			public:
				runnable_buffer_allocator() throw() {}
				runnable_buffer_allocator( runnable_buffer_allocator const& ) throw() {}
				template<typename U>
				runnable_buffer_allocator( runnable_buffer_allocator<U>& ) throw() {}

				~runnable_buffer_allocator() throw() {}
	

				pointer allocate( size_type num, void const* hint = 0 )
				{
					pointer const p = reinterpret_cast<pointer>(
						::VirtualAlloc( NULL, num * sizeof(T), MEM_COMMIT, PAGE_EXECUTE_READWRITE )
						);
					if ( p == NULL )
						throw std::bad_alloc();

					return p;
				}

				void construct( pointer p, T const& value )
				{
					new( reinterpret_cast<void*>( p ) ) T( value );
				}
	
				void deallocate( pointer p, size_type num )
				{
					::VirtualFree( p, 0, MEM_RELEASE );
				}

				void destroy(pointer p)
				{
					p->~T();
				}
	
				pointer address( reference value )
				{
					return &value;
				}

				const_pointer address( const_reference value ) const
				{
					return &value;
				}
	
				size_type max_size() const throw()
				{
					return (std::numeric_limits<size_type>::max)() / sizeof(T);
				}
			};

		} // namespace windows
	} // namespace assembeler
} // namespace ytl

#endif /*YTL_ASSEMBLER_WINDOWS_RUNNABLE_BUFFER_ALLOCATOR_HPP*/