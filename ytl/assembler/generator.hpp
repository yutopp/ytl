#ifndef YTL_ASSEMBLER_GENERATOR_HPP
#define YTL_ASSEMBLER_GENERATOR_HPP

#include <type_traits>
#include <memory>
#include <functional>

#include <boost/proto/proto.hpp>
#include <boost/noncopyable.hpp>

#include <ytl/utility.hpp>

#include "config.hpp"
#include "writer.hpp"
#include "endian_writer.hpp"
#include "binary.hpp"

namespace ytl
{
	namespace assembler
	{
		template<
			template<typename> class Engine,
			template<typename, typename> class Writer = variable_writer,
			typename EndianWritter = assembler::endian::little
		>
		class generator
			: private boost::noncopyable
		{
			typedef std::size_t											index_type;
			typedef std::shared_ptr<index_type const>					index_const_ptr_type;
			typedef EndianWritter										endian_writer_type;
			
			class index_wrapper;
			typedef index_wrapper&										index_wrapper_ref_type;
			typedef typename
				boost::proto::terminal<index_wrapper_ref_type>::type	lazy_index_type;

		public:
			generator()
				: index_wrapper_()
				, $( lazy_index_type::make( std::ref( index_wrapper_ ) ) )
			{}

			template<typename Buffer>
			Engine<Writer<Buffer, endian_writer_type>> // engine_type
			operator()( Buffer& buffer,
						index_type const index = 0u,
						typename std::enable_if<!ytl::has_wrapped_container_type<Buffer>::value>::type* =0 )
			{
				typedef typename Writer<Buffer, endian_writer_type>	writer_type;
				typedef typename Engine<writer_type>				engine_type;

				auto const writer( std::make_shared<writer_type>( buffer, index ) );
				std::shared_ptr<detail::asm_status> const status( new detail::asm_status );

				index_wrapper_ = index_wrapper( writer->get_index_ptr() );
				
				return engine_type( writer, status /*std::make_shared<detail::asm_status>()*/ );
			}

			template<typename Buffer>
			Engine<Writer<typename Buffer::wrapped_container_type, endian_writer_type>> // engine_type
			operator()( Buffer& buffer,
						index_type const index = 0u,
						typename std::enable_if<ytl::has_wrapped_container_type<Buffer>::value>::type* =0 )
			{
				typedef typename Writer<
					typename Buffer::wrapped_container_type,
					endian_writer_type>								writer_type;
				typedef typename Engine<writer_type>				engine_type;

				auto const writer( std::make_shared<writer_type>( buffer.get(), index ) );

				index_wrapper_ = index_wrapper( writer->get_index_ptr() );

				return engine_type( writer, buffer.get_status() );
			}

		private:
			class index_wrapper
			{
			public:
				index_wrapper()
					: p_( nullptr )
				{}

				explicit index_wrapper( index_const_ptr_type p )
					: p_( p )
				{}

				template<typename T>
				index_type operator-( T const& rhs ) const
				{
					return *p_ - rhs
				}

				template<typename T>
				friend index_type operator-( T const& lhs, index_wrapper const& rhs )
				{
					return lhs - *rhs.p_;
				}

			private:
				index_const_ptr_type p_;
			} index_wrapper_;

		public:
			lazy_index_type $;
		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_GENERATOR_HPP*/