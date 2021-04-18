#pragma once
#include <type_traits>


template<typename _Type1, typename _Type2>
class is_equatable
{
    template<typename __Type1, typename __Type2>
    static auto test(int)
        -> decltype(std::declval<__Type1>() == std::declval<__Type2>(), std::true_type());

    template<typename, typename>
    static auto test(...)->std::false_type;

public:
    static const bool value = decltype(test<_Type1, _Type2>(0))::value;
};

template<typename _Type1, typename _Type2>
inline constexpr bool is_equatable_v = is_equatable<_Type1, _Type2>::value;

template<typename _Type>
using is_equatable_self = is_equatable<_Type, _Type>;

template<typename _Type>
inline constexpr bool is_equatable_self_v = is_equatable_self<_Type>::value;