#ifndef YTL_BRAINFUCK_INTERPRETER_HPP
#define YTL_BRAINFUCK_INTERPRETER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iterator>

namespace ytd
{
	namespace brainfuck
	{
		namespace detail
		{
			struct default_dictionary
			{
				typedef char char_type;
				static int const max_length = 1;

				static char const* increment_pointer()
				{
					return ">";
				}

				static char const* decrement_pointer()
				{
					return "<";
				}

				static char const* repeat()
				{
					return "[";
				}

				static char const* loop()
				{
					return "]";
				}

				static char const* increment_value()
				{
					return "+";
				}

				static char const* decrement_value()
				{
					return "-";
				}

				static char const* input()
				{
					return ",";
				}

				static char const* output()
				{
					return ".";
				}
			};
		} // namespace detail

		template<typename DictT>
		struct dictionary_traits
		{
			typedef typename DictT::char_type char_type; 
			static int const max_length = DictT::max_length;

			static char_type const* increment_pointer()
			{
				return DictT::increment_pointer();
			}

			static char_type const* decrement_pointer()
			{
				return DictT::decrement_pointer();
			}

			static char_type const* repeat()
			{
				return DictT::repeat();
			}

			static char_type const* loop()
			{
				return DictT::loop();
			}

			static char_type const* increment_value()
			{
				return DictT::increment_value();
			}

			static char_type const* decrement_value()
			{
				return DictT::decrement_value();
			}

			static char_type const* input()
			{
				return DictT::input();
			}

			static char_type const* output()
			{
				return DictT::output();
			}
		};

		template<typename IterT, typename DictT = detail::default_dictionary, typename ElemT = unsigned char, std::size_t MemLimit = 30000>
		class interpreter
		{
			typedef DictT dictinary_type;
			typedef std::basic_string<typename std::iterator_traits<IterT>::value_type> string_type;
			typedef ElemT value_type;
			typedef typename std::vector<value_type> container;
			typedef typename container::iterator iterator;
			struct order
			{
				typedef int type;
				enum {
					finish = 0,
					increment_pointer,
					decrement_pointer,
					repeat,
					loop,
					increment_value,
					decrement_value,
					input,
					output,
					nop
				};
			};

		public:
			interpreter( IterT const& begin, IterT const& end )
				: memory_( MemLimit )
				, ptr_( memory_.begin() )
				, begin_( begin )
				, end_( end )
			{}

			IterT run()
			{
				eval( begin_, end_ );
				return begin_;
			}

		private:
			bool eval( IterT& it, IterT const& end )
			{
				while( it != end ) {
					const auto order = get_token( it, end );
					const auto& type = order.first;

					if ( type == order::increment_pointer ) {
						++ptr_;
						if ( ptr_ == memory_.cend() )
							return false;

					} else if ( type == order::decrement_pointer ) {
						if ( ptr_ == memory_.cbegin() )
							return false;
						--ptr_;

					} else if ( type == order::repeat ) {
						if ( *ptr_ ) {
							auto tmp = it;
							std::advance( tmp, order.second );
							if ( !eval( tmp, end ) )
								return false;
					
						} else {
							int depth = 0;
							for(;;) {
								const auto order = get_token( it, end );
								if ( order.first == order::finish )
									return false;

								std::advance( it, order.second );
								if ( order.first == order::loop ) {
									--depth;
									if ( depth == 0)
										break;
								} else if ( order.first == order::repeat ) {
									++depth;
								}
							}
						}
						continue;	

					} else if ( type == order::loop ) {
						return true;

					} else if ( type == order::increment_value ) {
						++(*ptr_);

					} else if ( type == order::decrement_value ) {
						--(*ptr_);

					} else if ( type == order::input ) {
						std::cin >> *ptr_;

					} else if ( type == order::output ) {
						std::cout << *ptr_;

					}/* else {
						// nop
					}*/

					std::advance( it, order.second );

					if ( type == order::finish )
						break;
				}

				return true;
			}

			std::pair<typename order::type, std::size_t> get_token( IterT const& begin, IterT const& end ) const
			{
				typedef dictionary_traits<dictinary_type> dic_type;
				string_type order;
				std::size_t retry = 0;
				for( IterT it=begin; it!=end; ++it, ++retry ) {
					if ( retry >= dic_type::max_length )
						return std::make_pair( order::nop, 1 );

					order += *it;

					if ( order == dic_type::increment_pointer() ) {
						return std::make_pair( order::increment_pointer, order.size() );

					} else if ( order == dic_type::decrement_pointer() ) {
						return std::make_pair( order::decrement_pointer, order.size() );

					} else if ( order == dic_type::repeat() ) {
						return std::make_pair( order::repeat, order.size() );

					} else if ( order == dic_type::loop() ) {
						return std::make_pair( order::loop, order.size() );

					} else if ( order == dic_type::increment_value() ) {
						return std::make_pair( order::increment_value, order.size() );

					} else if ( order == dic_type::decrement_value() ) {
						return std::make_pair( order::decrement_value, order.size() );

					} else if ( order == dic_type::output() ) {
						return std::make_pair( order::output, order.size() );

					} else if ( order == dic_type::input() ) {
						return std::make_pair( order::input, order.size() );
					}
				}

				return std::make_pair( order::finish, order.size() );
			}

			container memory_;
			iterator ptr_;
			IterT begin_, end_;
		};

		//
		template<typename IterT>
		bool parse( IterT const& begin, IterT const& end )
		{
			interpreter<IterT> bf = interpreter<IterT>( begin, end );
			return bf.run();
		}

		template<typename DictT, typename IterT>
		bool parse( IterT const& begin, IterT const& end )
		{
			interpreter<IterT, DictT> bf = interpreter<IterT, DictT>( begin, end );
			return bf.run() == end;
		}

		template<typename IterT>
		bool parse( IterT& begin, IterT const& end )
		{
			interpreter<IterT> bf = interpreter<IterT>( begin, end );
			begin = bf.run();
			return begin == end;
		}

		template<typename DictT, typename IterT>
		bool parse( IterT& begin, IterT const& end )
		{
			interpreter<IterT, DictT> bf = interpreter<IterT, DictT>( begin, end );
			begin = bf.run();
			return begin == end;
		}

	} // namespace brainfuck
} // namespace ytd

#endif /*YTL_BRAINFUCK_INTERPRETER_HPP*/