#pragma once
#include <type_traits>

template <typename _Compare, typename _Left, typename _Right = _Left>
class is_comparison
{
    template<typename __Compare, typename __Left, typename __Right>
    static auto test(int)
        -> decltype(std::invoke(std::declval<__Compare>(), std::declval<__Left>(), std::declval<__Right>()) ? 0 : 0, std::true_type());

    template<typename, typename, typename>
    static auto test(...)->std::false_type;

public:
    static const bool value = decltype(test<_Compare, _Left, _Right>(0))::value;
};

template <typename _Compare, typename _Left, typename _Right = _Left>
inline constexpr bool is_comparison_v = is_comparison<_Compare, _Left, _Right>::value;

template <typename _Compare, typename _Left, typename _Right = _Left, std::enable_if_t<is_comparison_v<_Compare, _Left, _Right>, bool> = true>
auto cast_static_comparison(_Compare comparer)
{
    return [comparer](_Left left, _Right right) -> bool { return std::invoke(comparer, left, right); };
}