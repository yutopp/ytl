#ifndef YTL_FILE_MAPPED_BINARY_BUFFER_HPP
#define YTL_FILE_MAPPED_BINARY_BUFFER_HPP

#include <utility>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/noncopyable.hpp>

#include "detail/binary_buffer_base.hpp"
#include "detail/binary_range_traits.hpp"

namespace ytl
{
	// file mapped binary buffer
	class file_mapped_binary_buffer
		: public detail::fixed_buffer_base<
					file_mapped_binary_buffer, detail::unused_allocator,
					detail::binary_range_container
				>
		, private boost::noncopyable
	{
	public:
		typedef file_mapped_binary_buffer				self_type;
		typedef self_type::base_type					base_type;

		typedef base_type::wrapped_container_type		wrapped_container_type;

	public:
		file_mapped_binary_buffer( char const* file_path, boost::interprocess::mode_t mode = boost::interprocess::read_only )
			: memmap_( file_path, mode )
			, view_( memmap_, mode )
			, rng_(
					static_cast<value_type*>( view_.get_address() ),
					static_cast<value_type*>( view_.get_address() ) + view_.get_size()
					)
		{}

		file_mapped_binary_buffer( file_mapped_binary_buffer&& rhs )
			: memmap_( std::move( rhs.memmap_ ) )
			, view_( std::move( rhs.view_ ) )
			, rng_( std::move( rhs.rng_ ) )
		{}

		file_mapped_binary_buffer& operator=( file_mapped_binary_buffer&& rhs )
		{
			if ( this != &rhs ) {
				memmap_ = std::move( rhs.memmap_ );
				view_ = std::move( rhs.view_ );
				rng_ = std::move( rhs.rng_ );
			}

			return *this;
		}


		wrapped_container_type& get()
		{
			return rng_;
		}

		wrapped_container_type const& get() const
		{
			return rng_;
		}

		void swap( self_type& rhs )
		{
			using std::swap;

			swap( memmap_, rhs.memmap_ );
			swap( view_, rhs.view_ );
			swap( rng_, rhs.rng_ );
		}

	private:
		boost::interprocess::file_mapping memmap_;
		boost::interprocess::mapped_region view_;

		wrapped_container_type rng_;
	};

} // namespace ytl

#endif /*YTL_FILE_MAPPED_BINARY_BUFFER_HPP*/