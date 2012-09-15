#ifndef YTL_FILEFORMAT_DETAIL_HPP
#define YTL_FILEFORMAT_DETAIL_HPP

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace ytl
{
	namespace fileformat
	{
		namespace detail
		{
			template<typename T, std::size_t N>
			inline std::string shrink_space( T const (&bin)[N] )
			{
				return shrink_space( reinterpret_cast<char const*>( bin ), reinterpret_cast<char const*>( bin + N ) );
			}

			template<typename IterT>
			inline std::string shrink_space( IterT const& begin, IterT const& end )
			{
				return boost::algorithm::replace_all_copy(
									std::string( begin, end )
									, " "
									, ""
									);
			}


			template<typename ReturnT, typename T, std::size_t N>
			inline ReturnT binary_cast( T const (&bin)[N] )
			{
				return binary_cast<ReturnT>( reinterpret_cast<char const*>( bin ), reinterpret_cast<char const*>( bin + N ) );
			}

			template<typename ReturnT, typename IterT>
			inline ReturnT binary_cast( IterT const& begin, IterT const& end )
			{
				return boost::lexical_cast<ReturnT>( shrink_space( begin, end ) );
			}
		} // namespace detail
	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_DETAIL_HPP*/