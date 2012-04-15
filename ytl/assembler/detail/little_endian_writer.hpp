#ifndef YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_WRITER_HPP
#define YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_WRITER_HPP

#include "../config.hpp"

namespace ytd
{
	namespace assembler
	{
		namespace detail
		{
			/*
			*/
			template<std::size_t N>
			struct little_endian_writer_impl;

			/*
			*/
			struct little_endian_writer
			{
				template<typename T>
				static void write( byte_t* const pointer, T const* const value )
				{
					little_endian_writer_impl<sizeof(T)>::write( pointer, value );
				}
			};


			/*
			*/
			template<>
			struct little_endian_writer_impl<1>
			{
				static void write( byte_t* const pointer, void const* const value )
				{
					std::memcpy( pointer, value, 1 );
				}
			};

			template<>
			struct little_endian_writer_impl<2>
			{
				static void write( byte_t* const pointer, void const* const value )
				{
					std::memcpy( pointer, value, 2 );
				}
			};

			template<>
			struct little_endian_writer_impl<4>
			{
				static void write( byte_t* const pointer, void const* const value )
				{
					std::memcpy( pointer, value, 4 );
				}
			};

		} // namespace detail
	} // namespace assembler
} // namespace ytd

#endif /*YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_WRITER_HPP*/
