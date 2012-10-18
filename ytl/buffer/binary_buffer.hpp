#ifndef YTL_BINARY_BUFFER_HPP
#define YTL_BINARY_BUFFER_HPP

#include <vector>

#include "detail/binary_buffer_base.hpp"

namespace ytl
{
	namespace detail
	{
		template<typename Value, template<typename> class Allocator>
		struct binary_buffer_container
		{
			typedef std::vector<Value, Allocator<Value>>		type;
		};

	} // namespace detail


	// binary buffer
	template<template <typename> class Allocator = std::allocator>
	class binary_buffer
		: public detail::resizable_buffer_base<
					binary_buffer<Allocator>, Allocator,
					detail::binary_buffer_container
				>
	{
	public:
		typedef binary_buffer									self_type;
		typedef typename self_type::base_type					base_type;

		typedef typename base_type::wrapped_container_type		wrapped_container_type;

		typedef typename base_type::size_type					size_type;
		typedef typename base_type::value_type					value_type;

	public:
		binary_buffer()
		{}

/*		binary_buffer( size_type const size )
			: buffer_( size )
		{}*/

/*		template<typename IterT>
		binary_buffer( IterT const& begin, IterT const& end )
			: buffer_( begin, end )
		{
//			std::cout << "basic_binary: iter ctor." << std::endl;
		}*/


		wrapped_container_type& get()
		{
			return buffer_;
		}

		wrapped_container_type const& get() const
		{
			return buffer_;
		}

		void swap( self_type& rhs )
		{
			using std::swap;

			swap( buffer_, rhs.buffer_ );
		}


		void align( size_type alignment, value_type v = 0 )
		{
			size_type const rest = buffer_.size() % alignment;

			for( size_type i=0; i<rest; ++i ) {
				buffer_.emplace_back( v );
			}
		}

	private:
		wrapped_container_type buffer_;
	};
}

#endif /*YTL_BINARY_BUFFER_HPP*/