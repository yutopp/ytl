#ifndef YTL_ASSEMBLER_GENERATOR_HPP
#define YTL_ASSEMBLER_GENERATOR_HPP

#include <type_traits>
#include <functional>

#include <boost/noncopyable.hpp>

#include "config.hpp"
#include "writer.hpp"
#include "endian_writer.hpp"

namespace ytl
{
	namespace assembler
	{
		template<
			template<typename> class Engine,
			typename Buffer,
			template<typename, typename> class Writer = variable_writer,
			typename EndianWritter = assembler::endian::little
		>
		class generator
			: private boost::noncopyable
		{
			typedef Buffer									buffer_type;
			typedef EndianWritter							endian_writer_type;
			typedef Writer<buffer_type, endian_writer_type>	writer_type;
			typedef typename writer_type::index_type		index_type;
			typedef Engine<writer_type>						engine_type;
			
		public:
			generator( buffer_type& buffer )
				: writer_( buffer )
				, $( writer_.get_index_ref() )
			{}

			//
			engine_type operator()()
			{
				return engine_type( std::ref( writer_ ) );
			}

		private:
			writer_type writer_;

		public:
			index_type const& $;	// always pointing buffer index.
		};

		template<template<typename> class Engine, typename Buffer>
		generator<Engine, Buffer> make_generator( Buffer& b )
		{
			return generator<Engine, Buffer>( b );
		}

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_GENERATOR_HPP*/