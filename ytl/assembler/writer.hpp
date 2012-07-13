#ifndef YTL_ASSEMBLER_WRITER_HPP
#define YTL_ASSEMBLER_WRITER_HPP

#include <cstddef>

#include "config.hpp"
#include "detail/buffer_wrapper.hpp"

namespace ytl
{
	namespace assembler
	{
		template<typename Buffer, typename EndianWriter>
		class fixed_writer
		{
			typedef buffer_wrapper<Buffer>				wrapper_type;
			typedef EndianWriter						writer_type;
			static_assert(
				sizeof( typename wrapper_type::value_type ) == 1,
				__FILE__ " required element of buffer is 1."
				);

		public:
			typedef typename wrapper_type::index_type	index_type;

		public:
			fixed_writer( Buffer& b, index_type const index = 0u )
				: wrapper_( b, index )
			{}

			template<typename T>
			void write( T const& v, std::size_t const size = sizeof(T) )
			{
				if ( wrapper_.size() <= wrapper_.index_ + size )
					throw std::exception();		// todo: cause error

				writer_type::write(
					reinterpret_cast<byte_t*>( wrapper_.data() ) + wrapper_.index_,
					std::addressof( v ),
					size
					);
				wrapper_.index_ += size;
			}

			index_type const& get_index_cref() const
			{
				return wrapper_.index_;
			}

		private:
			wrapper_type wrapper_;
		};


		template<typename Buffer, typename EndianWriter>
		class variable_writer
		{
			typedef buffer_wrapper<Buffer>				wrapper_type;
			typedef EndianWriter						writer_type;
			static_assert(
				sizeof( typename wrapper_type::value_type ) == 1,
				__FILE__ " required element of buffer is 1."
				);

		public:
			typedef typename wrapper_type::index_type	index_type;

		public:
			variable_writer( Buffer& b, index_type const index = 0u )
				: wrapper_( b, index )
			{}

			template<typename T>
			void write( T const& v, std::size_t const size = sizeof(T) )
			{
				extend_if_insufficient_buffer( size );

				writer_type::write(
					reinterpret_cast<byte_t*>( wrapper_.data() ) + wrapper_.index_,
					std::addressof( v ),
					size
					);
				wrapper_.index_ += size;
			}

			index_type const& get_index_cref() const
			{
				return wrapper_.index_;
			}

		private:
			inline void extend_if_insufficient_buffer( std::size_t const size )
			{
				if ( wrapper_.size() <= wrapper_.index_ + size )
					wrapper_.resize( wrapper_.size() + size );
			}

			wrapper_type wrapper_;
		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_WRITER_HPP*/