#ifndef YTL_SHARED_BINARY_RANGE_HPP
#define YTL_SHARED_BINARY_RANGE_HPP

#include <memory>

#include "detail/binary_buffer_base.hpp"
#include "detail/binary_range_traits.hpp"

namespace ytl
{
	template<typename Owner>
	class const_shared_binary_range;

	namespace detail
	{
		template<typename Owner>
		struct container_copy_traits<const_shared_binary_range<Owner>>
		{
			typedef Owner									owner_type;
			typedef std::shared_ptr<owner_type const>		owner_shared_pointer;

			typedef const_shared_binary_range<Owner>		container_type;
            typedef typename container_type::const_pointer  binary_const_pointer;

		public:
			static container_type copy( 
                owner_shared_pointer const& owner,
                binary_const_pointer const& begin,
                binary_const_pointer const& end
                )
			{
				return container_type( owner, begin, end );
			}
		};

	} // namespace detail


	template<typename Owner>
	class const_shared_binary_range
		: public detail::fixed_buffer_base<
					const_shared_binary_range<Owner>, detail::unused_allocator,
					detail::const_binary_range_container
				>
	{
	public:
		typedef const_shared_binary_range							self_type;
		typedef typename self_type::base_type						base_type;

		typedef typename base_type::wrapped_container_type			range_type;
		typedef typename base_type::const_pointer					const_pointer;

		typedef Owner												owner_type;
		typedef std::shared_ptr<owner_type const>					owner_shared_pointer;

	public:
		const_shared_binary_range() {}

		const_shared_binary_range( owner_shared_pointer ptr, const_pointer begin, const_pointer end )
			: ptr_( ptr )
			, rng_( begin, end )
		{}

		const_shared_binary_range( owner_shared_pointer ptr, range_type rng )
			: ptr_( ptr )
			, rng_( rng )
		{}

/*		const_shared_binary_range( const_shared_binary_range&& rhs )
			: ptr_( std::move( rhs.ptr_ ) )
			, rng_( std::move( rhs.rng_ ) )
		{}*/

		range_type& get()
		{
			return rng_;
		}

		range_type const& get() const
		{
			return rng_;
		}

		void swap( self_type& rhs )
		{
			using std::swap;

			swap( ptr_, rhs.ptr_ );
			swap( rng_, rhs.rng_ );
		}

	private:
		owner_shared_pointer ptr_;
		range_type rng_;
	};

} // namespace ytl

#endif /*YTL_SHARED_BINARY_RANGE_HPP*/