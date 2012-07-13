#ifndef YTL_ASSEMBLER_GENERATOR_HPP
#define YTL_ASSEMBLER_GENERATOR_HPP

#include <type_traits>
#include <memory>
#include <functional>

#include <boost/proto/proto.hpp>
#include <boost/noncopyable.hpp>

#include "config.hpp"
#include "writer.hpp"
#include "endian_writer.hpp"

namespace ytl
{
	namespace assembler
	{
		struct append
		{};

		struct overwrite
		{
			explicit overwrite( std::size_t const pos )
				: position( pos )
			{}

			std::size_t position;
		};

		template<
			template<typename> class Engine,
			template<typename, typename> class Writer = variable_writer,
			typename EndianWritter = assembler::endian::little
		>
		class generator
			: private boost::noncopyable
		{
			typedef std::size_t			index_type;
			typedef index_type const&	index_cref_type;
			typedef typename boost::proto::terminal<
				index_cref_type>::type	lazy_index_cref_type;
			typedef EndianWritter		endian_writer_type;

		public:
			template<typename Buffer>
			Engine<std::shared_ptr<Writer<Buffer, endian_writer_type>>> // engine_type
			operator()( Buffer& buffer, append const = append() )
			{
				typedef typename Writer<Buffer, endian_writer_type>	writer_type;
				typedef typename std::shared_ptr<writer_type>		writer_pointer_type;
				typedef typename Engine<writer_pointer_type>		engine_type;

				writer_pointer_type const writer_pointer(
					std::make_shared<writer_type>( buffer, 0u )
					);
				$ = lazy_index_cref_type::make( writer_->get_index_cref() );

				return engine_type( writer_ );
			}

			template<typename Buffer>
			Engine<std::shared_ptr<Writer<Buffer, endian_writer_type>>> // engine_type
			operator()( Buffer& buffer, overwrite const tag )
			{
				typedef typename Writer<Buffer, endian_writer_type>	writer_type;
				typedef typename std::shared_ptr<writer_type>		writer_pointer_type;
				typedef typename Engine<writer_pointer_type>		engine_type;

				writer_pointer_type const writer_pointer(
					std::make_shared<writer_type>( buffer, tag.position )
					);
				$ = lazy_index_cref_type::make( writer_->get_index_cref() );

				return engine_type( writer_ );
			}

		public:
			lazy_index_cref_type $;
		};

/*		template<template<typename> class Engine, typename Buffer>
		generator<Engine, Buffer> make_generator( Buffer& b )
		{
			return generator<Engine, Buffer>( b );
		}*/

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_GENERATOR_HPP*/