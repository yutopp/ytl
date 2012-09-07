#ifndef YTL_ASSEMBLER_WRITER_HPP
#define YTL_ASSEMBLER_WRITER_HPP

#include <cstddef>

#include <ytl/utility/guard_macros.hpp>

#include "config.hpp"
#include "detail/buffer_wrapper.hpp"

namespace ytl
{
	namespace assembler
	{
		template<typename Buffer, typename EndianWriter>
		class variable_writer
		{
			typedef detail::buffer_wrapper<Buffer>		wrapper_type;
			typedef EndianWriter						writer_type;

			YTL_REQUIRE_BINARY_BUFFER( wrapper_type )

		public:
			typedef typename wrapper_type::index_type					index_type;
			typedef typename wrapper_type::index_pointer_type			index_pointer_type;
			typedef typename wrapper_type::index_const_pointer_type		index_const_pointer_type;

		public:
			variable_writer( Buffer& b, index_type const index = 0u )
				: wrapper_( b, index )
			{}

			template<typename T>
			void write( T const* const p, std::size_t const size = sizeof(T) )
			{
				extend_if_insufficient_buffer( size );

				writer_type::write(
					reinterpret_cast<byte_t*>( wrapper_.data() ) + *wrapper_.index_,
					p,
					size
					);
				*wrapper_.index_ += size;
			}

			index_const_pointer_type get_index_ptr() const
			{
				return wrapper_.index_;
			}

		private:
			inline void extend_if_insufficient_buffer( std::size_t const size )
			{
				if ( wrapper_.size() <= *wrapper_.index_ + size )
					wrapper_.resize( wrapper_.size() + size );
			}

			wrapper_type wrapper_;
		};


		template<typename Buffer, typename EndianWriter>
		class fixed_writer
		{
			typedef detail::buffer_wrapper<Buffer>		wrapper_type;
			typedef EndianWriter						writer_type;

			YTL_REQUIRE_BINARY_BUFFER( wrapper_type )

		public:
			typedef typename wrapper_type::index_type					index_type;
			typedef typename wrapper_type::index_pointer_type			index_pointer_type;
			typedef typename wrapper_type::index_const_pointer_type		index_const_pointer_type;

		public:
			fixed_writer( Buffer& b, index_type const index = 0u )
				: wrapper_( b, index )
			{}

			template<typename T>
			void write( T const* const p, std::size_t const size = sizeof(T) )
			{
				if ( wrapper_.size() <= *wrapper_.index_ + size )
					throw std::exception();		// todo: cause error

				writer_type::write(
					reinterpret_cast<byte_t*>( wrapper_.data() ) + *wrapper_.index_,
					p,
					size
					);
				*wrapper_.index_ += size;
			}

			index_const_pointer_type get_index_ptr() const
			{
				return wrapper_.index_;
			}

		private:
			wrapper_type wrapper_;
		};



		// meta function
		template<typename Buffer, typename EndianWriter>
		struct suitable_writer
		{
		private:
			template<typename T>
			static auto check( T t ) 
				-> decltype( t.resize( std::declval<typename T::size_type>() ), std::declval<variable_writer<Buffer, EndianWriter>>() );

			template<typename T>
			static fixed_writer<Buffer, EndianWriter> check( ... );

		public:
			typedef decltype( check<Buffer>( std::declval<Buffer>() ) )		type;
		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_WRITER_HPP*/