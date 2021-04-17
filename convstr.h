#pragma once
#include <type_traits>
#include <ostream>
#include <sstream>

template<class _Type, class _Stream = std::ostringstream>
class is_streamable
{
    template<typename __Type, typename __Stream>
    static auto test(int)
        -> decltype(std::declval<__Stream&>() << std::declval<__Type>(), std::true_type());

    template<typename, typename>
    static auto test(...)->std::false_type;

public:
    static const bool value = decltype(test<_Type, _Stream>(0))::value;
};

template<class _Type, class _Stream = std::ostringstream>
inline constexpr bool is_streamable_v = is_streamable<_Type, _Stream>::value;

template<class T, std::enable_if_t<std::is_convertible_v<T, std::string>, bool> = true>
std::string convert_string(T t) {
    return static_cast<std::string>(t);
}

template <
    class T,
    std::enable_if_t<
        std::conjunction_v<
            std::negation<std::is_convertible<T, std::string>>,
            is_streamable<T>
        >,
        bool
    > = true
>
std::string convert_string(T t) {
    std::ostringstream stream = std::ostringstream();
    stream << t;
    return stream.str();
}

template<class T>
using can_convert_string = std::disjunction<std::is_convertible<T, std::string>, is_streamable<T>>;

template<class T>
inline constexpr bool can_convert_string_v = can_convert_string<T>::value;