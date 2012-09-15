#ifndef YTL_FILE_MAPPED_BINARY_BUFFER_HPP
#define YTL_FILE_MAPPED_BINARY_BUFFER_HPP

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/range/iterator_range.hpp>

#include "detail/binary_buffer_base.hpp"

namespace ytl
{
	namespace detail
	{
		template<typename Value, typename Unused_Allocator>
		struct file_mapped_binary_buffer_container
		{
			typedef boost::iterator_range<Value*>		type;
		};

		//
		template<typename Unused>
		struct unused_allocator
		{};

	} // namespace detail


	// file mapped binary buffer
	template<template <typename> class Allocator = detail::unused_allocator>
	class file_mapped_binary_buffer
		: public detail::fixed_buffer_base<
					file_mapped_binary_buffer, Allocator,
					detail::file_mapped_binary_buffer_container
				>
	{
	public:
		typedef file_mapped_binary_buffer						self_type;
		typedef typename self_type::base_type					base_type;

		typedef typename base_type::wrapped_container_type		wrapped_container_type;

	private:
		class mapping
		{
		public:
			mapping( char const* file_path, boost::interprocess::mode_t mode )
				: memmap( file_path, mode )
				, view( memmap, mode )
			{}

		public:
			boost::interprocess::file_mapping memmap;
			boost::interprocess::mapped_region view;
		};

	public:
		file_mapped_binary_buffer( char const* file_path, boost::interprocess::mode_t mode = boost::interprocess::read_only )
			: mapping_( std::make_shared<mapping>( file_path, mode ) ) 
			, rng_(
					static_cast<value_type*>( mapping_->view.get_address() ),
					static_cast<value_type*>( mapping_->view.get_address() ) + mapping_->view.get_size()
					)
		{}

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

			swap( mapping_, rhs.mapping_ );
			swap( rng_, rhs.rng_ );
		}

	private:
		std::shared_ptr<mapping> mapping_;
		wrapped_container_type rng_;
	};

} // namespace ytl

#endif /*YTL_FILE_MAPPED_BINARY_BUFFER_HPP*/