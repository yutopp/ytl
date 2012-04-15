#ifndef YTL_ASSEMBLER_CODE_STUB_HPP
#define YTL_ASSEMBLER_CODE_STUB_HPP

#include "config.hpp"

#include <cstddef>
#include <cstring>
#include <vector>
#include <exception>

namespace ytd
{
	namespace assembler
	{
		namespace detail
		{
			namespace windows
			{
				#include <windows.h>
				//#define win_min min
				//#define win_max max
				#undef min
				#undef max
				#undef far
			}

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

				/*
				*/
				template<typename U>
				struct rebind
				{
					typedef runnable_buffer_allocator<U> other;
				};
	
				/*
				*/
				runnable_buffer_allocator() throw() {}
				runnable_buffer_allocator( runnable_buffer_allocator const& ) throw() {}
				template<typename U>
				runnable_buffer_allocator( runnable_buffer_allocator<U>& ) throw() {}
				~runnable_buffer_allocator() throw() {}
	
				/*
				*/
				pointer allocate( size_type num, void const* hint = 0 )
				{
					pointer const p = reinterpret_cast<pointer>(
						windows::VirtualAlloc( NULL, num * sizeof(T), MEM_COMMIT, PAGE_EXECUTE_READWRITE )
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
					windows::VirtualFree( p, 0, MEM_RELEASE );
				}

				void destroy(pointer p)
				{
					p->~T();
				}
	
				pointer address( reference value ) const
				{
					return &value;
				}
				const_pointer address( const_reference value ) const
				{
					return &value;
				}
	
				size_type max_size() const throw()
				{
					return std::numeric_limits<size_t>::max() / sizeof(T);
				}
			};

		} // namespace detail

		//
		typedef std::vector<byte_t, detail::runnable_buffer_allocator<byte_t>> code_stub;

	} // namespace assembeler
} // namespace ytd

#endif /*YTL_ASSEMBLER_CODE_STUB_HPP*/