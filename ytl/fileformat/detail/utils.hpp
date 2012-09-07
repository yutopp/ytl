#ifndef YTL_FILEFORMAT_DETAIL_UTILS_HPP
#define YTL_FILEFORMAT_DETAIL_UTILS_HPP

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>

#include <boost/lexical_cast.hpp>

namespace ytl
{
	namespace fileformat
	{
		namespace detail
		{
			template<typename IterT>
			inline std::string trim( IterT const& begin, IterT const& end )
			{
				return boost::algorithm::trim_copy_if(
					std::string( begin, end ),
					[]( char c ){ return c == ' ' || c == '\0'; }
					);
			}

			template<typename Range>
			inline std::string trim( Range const& rng )
			{
				using std::begin;
				using std::end;

				return trim( begin( rng ), end( rng ) );
			}

			template<typename T, std::size_t N>
			inline std::string trim( T const(&bin)[N] )
			{
				return trim( reinterpret_cast<T const*>( bin ), reinterpret_cast<T const*>( bin ) + N );
			}


			template<typename ReturnT, typename IterT>
			inline ReturnT binary_cast( IterT const& begin, IterT const& end )
			{
				return boost::lexical_cast<ReturnT>( trim( begin, end ) );
			}

			template<typename ReturnT, typename Range>
			inline ReturnT binary_cast( Range const& rng )
			{
				using std::begin;
				using std::end;

				return binary_cast( begin( rng ), end( rng ) );
			}

			template<typename ReturnT, typename T, std::size_t N>
			inline ReturnT binary_cast( T const(&bin)[N] )
			{
				return binary_cast<ReturnT>( reinterpret_cast<T const*>( bin ), reinterpret_cast<T const*>( bin ) + N );
			}

		} // namespace detail
	} // namespace fileformat
} // namespace ytl

#endif /*#define YTL_FILEFORMAT_DETAIL_UTILS_HPP*/