#ifndef YTL_ASSEMBLER_ARCH_UTILITY_HPP
#define YTL_ASSEMBLER_ARCH_UTILITY_HPP

#include <utility>

#include <boost/proto/proto.hpp>
#include <boost/proto/context.hpp>

namespace ytl { namespace assembler{
	namespace detail {
		template<typename T>
		inline T&& evaluate( T&& v )
		{
			return std::forward<T>( v );
		}

		template<typename Expr>
		inline auto evaluate(
			const Expr& e,
			typename std::enable_if<boost::proto::is_expr<Expr>::value>::type* =0
			) -> decltype( boost::proto::eval( e, boost::proto::default_context() ) )
		{
			boost::proto::default_context ctx;
			return boost::proto::eval( e, ctx );
		}
	}
}} // namespace ytl::assembler

#define YTL_ASM_OP_BEGIN_RAW(sig)	return_type& sig
#define YTL_ASM_OP_BEGIN(sig)		YTL_ASM_OP_BEGIN_RAW(sig) {
#define YTL_ASM_OP_END				return static_cast<return_type&>( *this ); }

#define YTL_ASM_OP_LAZY_1( name )								\
	template<typename Expr1>									\
	inline YTL_ASM_OP_BEGIN_RAW(								\
				name(											\
					Expr1 const& expr1,							\
					typename std::enable_if<					\
						boost::proto::is_expr<Expr1>::value		\
					>::type* =0									\
					)											\
			)													\
	{															\
		return name(											\
			ytl::assembler::detail::evaluate( expr1 )			\
			);													\
	}


#define YTL_ASM_OP_LAZY_2( name )								\
	template<typename Expr1, typename Expr2>					\
	inline YTL_ASM_OP_BEGIN_RAW(								\
				name(											\
					Expr1 const& expr1,							\
					Expr2 const& expr2,							\
					typename std::enable_if<					\
						boost::proto::is_expr<Expr1>::value		\
						|| boost::proto::is_expr<Expr2>::value	\
					>::type* =0									\
					)											\
			)													\
	{															\
		return name(											\
			ytl::assembler::detail::evaluate( expr1 ),			\
			ytl::assembler::detail::evaluate( expr1 )			\
			);													\
	}


#endif /*YTL_ASSEMBLER_ARCH_UTILITY_HPP*/