#ifndef YTL_BINARY_RANGE_TRAITS_HPP
#define YTL_BINARY_RANGE_TRAITS_HPP

#include <utility>
#include <type_traits>

#include <boost/range/iterator_range.hpp>

#include "binary_buffer_base.hpp"

namespace ytl
{
	namespace detail
	{
		template<typename Value, template<typename> class Unused_Allocator>
		struct binary_range_container
		{
			typedef boost::iterator_range<Value*>			type;
		};

		template<typename Value, template<typename> class Unused_Allocator>
		struct const_binary_range_container
		{
			typedef boost::iterator_range<Value const*>		type;
		};

		template<>
		struct data_member_traits<binary_range_container>
		{
			template<typename T>
			static auto data( T& rng )
				-> decltype( rng.begin() )
			{
				return rng.begin();
			}

			template<typename T>
			static auto data( T const& rng )
				-> typename std::add_pointer<
						typename std::add_const<
							typename std::remove_pointer<decltype( rng.begin() )>::type
						>::type
					>::type
			{
				return rng.begin();
			}
		};

		template<>
		struct data_member_traits<const_binary_range_container>
		{
			template<typename T>
			static auto data( T& rng )
				-> decltype( static_cast<T const&>( rng ).begin() )
			{
				return static_cast<T const&>( rng ).begin();
			}

			template<typename T>
			static auto data( T const& rng )
				-> typename std::add_pointer<
						typename std::add_const<
							typename std::remove_pointer<decltype( rng.begin() )>::type
						>::type
					>::type
			{
				return rng.begin();
			}
		};

	} // namespace detail
} // namespace ytl

#endif /*YTL_BINARY_RANGE_TRAITS_HPP*/