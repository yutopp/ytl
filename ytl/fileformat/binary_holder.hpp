#ifndef YTL_FILEFORMAT_BINARYHOLDER_HPP
#define YTL_FILEFORMAT_BINARYHOLDER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace ytl
{
	namespace fileformat
	{
		class binary_holder
		{
			typedef binary_holder			self_type;

		public:
			typedef char					byte_type;
			typedef std::vector<byte_type>	container_type;

			binary_holder()
			{}
			
			template<typename CharT>
			explicit binary_holder( CharT const* const filename )
			{
				load_file( std::basic_string<CharT>( filename ) );
			}

			template<typename CharT>
			explicit binary_holder( std::basic_string<CharT> const& filename )
			{
				load_file( filename );
			}

			template<typename IterT>
			binary_holder( IterT const& begin, IterT const& end )
				: binary_( begin, end )
			{}

			container_type& get() { return binary_; }
			container_type const& get() const { return binary_; }

			container_type& operator*() { return get(); }
			container_type const& operator*() const { return get(); }

			container_type* operator->() { return &get(); }
			container_type const* operator->() const { return &get(); }

		private:
			template<typename CharT>
			void load_file( std::basic_string<CharT> const& filename )
			{
				std::basic_ifstream<byte_type> ifs( filename, std::ios::binary );
				if ( !ifs )
					return;

				std::istreambuf_iterator<byte_type> const begin( ifs ), end;
				std::copy( begin, end, std::back_inserter( binary_ ) );
			}

			container_type binary_;
		};

		inline binary_holder& operator<<( binary_holder& lhs, binary_holder const& rhs )
		{
			lhs->insert( lhs->end(), rhs->cbegin(), rhs->cend() );
			return lhs;
		}

		template<std::size_t N>
		inline binary_holder& operator<<( binary_holder& lhs, binary_holder::byte_type const (&rhs)[N] )
		{
			lhs->insert( lhs->end(), rhs, rhs + N );
			return lhs;
		}


		inline std::ostream& operator<<( std::basic_ostream<binary_holder::byte_type>& os, binary_holder const& buffer )
		{
			os.write( &buffer->data()[0], buffer->size() );
			return os;
		}

	} // namespace fileformat
} // namespace ytl

#endif /*YTL_FILEFORMAT_BINARYHOLDER_HPP*/