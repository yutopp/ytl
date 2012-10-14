#ifndef YTL_FILEFORMAT_UTILITY_HPP
#define YTL_FILEFORMAT_UTILITY_HPP

#include <string>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <ytl/utility.hpp>
#include <ytl/buffer/binary_buffer.hpp>
#include <ytl/buffer/file_mapped_binary_buffer.hpp>

namespace ytl
{
	namespace fileformat
	{
		template<typename Object>
		Object mapping_object( char const* const filename )
		{
			typedef typename std::remove_reference<Object>::type	build_object_type;

			return build_object_type( ytl::mapping( filename ) );
		}


		template<template<typename Buffer> class Object, typename Buffer, typename CharT>
		Object<Buffer> build_object( CharT const* const filename )
		{
			typedef Object<Buffer>							build_object_type;

			return build_object_type( ytl::read<Buffer>( filename ) );
		}

		template<template<typename Buffer> class Object, typename Buffer, typename CharT>
		inline Object<Buffer> build_object( std::basic_string<CharT> const& filename )
		{
			return build_object<Object, Buffer>( filename.c_str() );
		}

		template<template<typename Buffer> class Object, typename CharT>
		Object<ytl::binary_buffer<>> build_object( CharT const* const filename )
		{
			return build_object<Object, ytl::binary_buffer<>>( filename );
		}

		template<template<typename Buffer> class Object, typename Buffer, typename CharT>
		inline Object<ytl::binary_buffer<>> build_object( std::basic_string<CharT> const& filename )
		{
			return build_object<Object>( filename.c_str() );
		}


		template<template<typename Buffer> class Object, typename Buffer>
		Object<Buffer> build_object( Buffer&& buffer )
		{
			typedef Object<Buffer>							build_object_type;

			return build_object_type( std::forward<Buffer>( buffer ) );
		}

#if 0
		namespace detail
		{
			struct read_function
			{
				template<typename Buffer>
				void operator()( Buffer& b ) const
				{
					ytl::read_binary( filename, b );
				}

				char const* filename;
			};

			template<typename IterT>
			struct make_function
			{
				template<typename Buffer>
				void operator()( Buffer& b ) const
				{
					b = Buffer( begin, end );
				}

				IterT begin, end;
			};
			
			template<typename Buffer>
			struct make_movable_function
			{
				template<typename Buffer>
				void operator()( Buffer& b ) const
				{
					b = std::move( buffer );
				}

				Buffer&& buffer;
			};
		} // namespace detail

		template<typename ObjectT, typename IterT>
		ObjectT make( IterT const& begin, IterT const& end )
		{
			typedef typename std::remove_cv<ObjectT>::type			object_type;
			typedef typename std::add_pointer<object_type>::type	pointer_type;

			detail::make_function<IterT> f= { begin, end };
			return build_object( f, static_cast<pointer_type>(0) );
		}

		template<typename ObjectT, typename Buffer>
		inline ObjectT make( Buffer const& buffer, typename std::enable_if<!ytl::has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			return make<ObjectT>( buffer.cbegin(), buffer.cend() );
		}

		template<typename ObjectT, typename Buffer>
		inline ObjectT make( Buffer const& buffer, typename std::enable_if<ytl::has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			return make<ObjectT>( *buffer );
		}

		template<typename ObjectT, typename Buffer>
		inline ObjectT make( Buffer&& buffer, typename std::enable_if<!ytl::has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			typedef typename std::remove_cv<ObjectT>::type			object_type;
			typedef typename std::add_pointer<object_type>::type	pointer_type;

			detail::make_movable_function<Buffer> f= { std::move( buffer ) };
			return build_object( f, static_cast<pointer_type>(0) );
		}

		template<typename ObjectT, typename Buffer>
		inline ObjectT make( Buffer&& buffer, typename std::enable_if<ytl::has_wrapped_container_type<Buffer>::value>::type* =0 )
		{
			return make<ObjectT>( std::move( *buffer ) );
		}
		
/*		template<typename ObjectT, typename Buffer>
		inline ObjectT make( std::shared_ptr<Buffer> const& buffer )
		{
			return make<ObjectT>( *buffer );
		}*/


		template<typename ObjectT, typename CharT>
		ObjectT read( CharT const* const filename )
		{
			typedef typename std::remove_cv<ObjectT>::type			object_type;
			typedef typename std::add_pointer<object_type>::type	pointer_type;

			detail::read_function f = { filename };
			return build_object( f, static_cast<pointer_type>( nullptr ) );
		}

		template<typename ObjectT, typename CharT>
		ObjectT read( std::basic_string<CharT> const& filename )
		{
			return read<ObjectT>( filename.c_str() );
		}


/*
		template<typename F>
		Dest build_object( F const& f, void* )
		{
			throw std::runtime_error( "Unreach build_object function[ void* ]" );
		}
*/
#endif

	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_UTILITY_HPP*/