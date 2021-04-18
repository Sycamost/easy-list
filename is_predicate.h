#pragma once
#include <type_traits>

template <typename _Predicate, typename _Type>
class is_predicate
{
	template <typename __Predicate, typename __Type>
	static auto test(int)
		-> decltype(std::declval<__Predicate>()(std::declval<__Type>()) ? 0 : 0, std::true_type());

	template <typename, typename>
	static auto test(...)->std::false_type;

public:
	static const bool value = decltype(test<_Predicate, _Type>(0))::value;
};

template <typename _Predicate, typename _Type>
inline constexpr bool is_predicate_v = is_predicate<_Predicate, _Type>::value;