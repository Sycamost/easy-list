#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <template_helpers.h>

namespace easy_list
{
    template<class _Type, class _Alloc = std::allocator<_Type>>
    class list : public std::vector<_Type, _Alloc>
    {
    public:
        using _Mybase = std::vector<_Type>;
        using value_type = _Type;
        using allocator_type = typename _Mybase::allocator_type;
        using size_type = typename _Mybase::size_type;

        ////////////////////
        /// CONSTRUCTORS ///
        ////////////////////

        list() : _Mybase() {}

        explicit list(const _Alloc& _Al) : _Mybase(_Al) {}

        list(const _Mybase& _Right, const _Alloc& _Al = _Alloc()) : _Mybase(_Right, _Al) {}

        list(const size_t _Count, const _Type& _Val, const _Alloc& _Al = _Alloc()) : _Mybase(_Count, _Val, _Al) {}

        template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        list(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc()) : _Mybase(_First, _Last, _Al) {}

        list(list&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase((_Mybase&&)_Right, _Al) {}

        list(_Mybase&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase(_Right, _Al) {}

        list(std::initializer_list<_Type> _Ilist, const _Alloc& _Al = _Alloc()) : _Mybase(_Ilist, _Al) {}

        list(const list& _Right, const _Alloc& _Al = _Alloc()) : _Mybase((const _Mybase&)_Right, _Al) {}

        ~list() { }


        //////////////////////////
        /// OPERATOR OVERLOADS ///
        //////////////////////////

        list& operator=(const list& _Right) {
            return _Mybase::operator=((const _Mybase&)_Right);
        }

        list& operator=(list&& _Right) noexcept {
            _Mybase::operator=((_Mybase&&)_Right);
            return *this;
        }

        template <std::enable_if_t<template_helpers::can_convert_string_v<_Type>, bool> = true>
        operator std::string() const
        {

            std::string str = "easy_list::list<";
            str += typeid(_Type).name();
            str += ">: (";
            if (_Mybase::size() != 0)
            {
                for (_Type elem : *this)
                    str += template_helpers::convert_string(elem) + ", ";
                str = str.substr(0, str.length() - 2);
            }
            str += ")";
            return str;
        }

        friend std::ostream& operator<<(std::ostream& output, const list& list)
        {
            std::string str = list;
            output << str;
            return output;
        }


        /////////////////
        /// SEARCHING ///
        /////////////////

        template <std::enable_if_t<template_helpers::is_equatable_self_v<_Type>, bool> = true>
        [[nodiscard]] typename _Mybase::iterator search(const _Type match)
        {
            return std::find(this->begin(), this->end(), match);
        }

        template <
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _MatchType>>,
                    template_helpers::is_equatable<_Type, _MatchType>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] typename _Mybase::iterator search(const _MatchType match)
        {
            return std::find(this->begin(), this->end(), [match](_Type other) -> bool { return other == match; });
        }

        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _Predicate>>,
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] typename _Mybase::iterator search(_Predicate predicate)
        {
            return std::find_if(this->begin(), this->end(), predicate);
        }

        template <
            typename _Result,
            typename _Callable,
            typename... _Args,
            std::enable_if_t<
                std::is_invocable_r_v<_Result, decltype(std::declval<_Callable>()), _Type, _Args...>,
                bool
            >
            = true
        >
        [[nodiscard]] typename _Mybase::iterator search(_Result match, _Callable member, const _Args&... args)
        {
            return std::find_if(
                this->begin(),
                this->end(),
                [match, member](_Type other)
                -> bool { return std::invoke(member, other, args...) == match; }
            );
        }


        ///////////////
        /// SORTING ///
        ///////////////

        template <typename _Compare, std::enable_if_t<template_helpers::is_comparison_v<_Compare, _Type>, bool> = true>
        object_list& sort(_Compare comparer)
        {
            std::sort(
                this->begin(),
                this->end(),
                template_helpers::cast_static_comparison<_Compare, _Type>(comparer)
            );
            return *this;
        }

        object_list& sort()
        {
            this->sort(std::less<>{});
            return *this;
        }

        template <
            typename _Compare,
            typename _Callable,
            typename... _Args,
            typename _Result = std::remove_reference_t<decltype(std::invoke(std::declval<_Callable>(), std::declval<_Type>(), std::declval<_Args>()...))>,
            std::enable_if_t<
                std::conjunction_v<
                    template_helpers::is_comparison<_Compare, _Result>,
                    template_helpers::is_const_member_of<_Callable, _Type, _Args...>
                >,
                bool
            >
            = true
        >
        object_list& sort(_Compare comparer, _Callable member, const _Args&... args)
        {
            inline auto static_comparer = template_helpers::cast_static_comparison<_Compare, _Type>(comparer);
            auto pred = [static_comparer, member, args...](const _Type& lhs, const _Type& rhs) -> auto {
                const _Result resultLhs = std::invoke(member, lhs, args...);
                const _Result resultRhs = std::invoke(member, rhs, args...);
                return static_comparer(resultLhs, resultRhs);
            };
            std::sort(this->begin(), this->end(), pred);
            return *this;
        }

        template <
            typename _Callable,
            typename... _Args
        >
        object_list& sort(_Callable member, const _Args&... args)
        {
            this->sort(std::less<>{}, member, args...);
            return *this;
        }


        /////////////////
        /// SELECTING ///
        /////////////////

        template <std::enable_if_t<template_helpers::is_equatable_self_v<_Type>, bool> = true>
        [[nodiscard]] list select(const _Type match)
        {
            list sublist = list();
            for (_Type elem : *this)
            {
                if (elem == match)
                    sublist.push_back(elem);
            }
            return sublist;
        }

        template <
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _MatchType>>,
                    template_helpers::is_equatable<_Type, _MatchType>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] list select(const _MatchType match)
        {
            list sublist = list();
            for (_Type elem : *this)
            {
                if (elem == match)
                    sublist.push_back(elem);
            }
            return sublist;
        }

        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _Predicate>>,
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] list select(_Predicate predicate)
        {
            list sublist = list();
            for (_Type elem : *this)
            {
                if (predicate(elem))
                    sublist.push_back(elem);
            }
            return sublist;
        }

        template <
            typename _Result,
            typename _Callable,
            typename... _Args,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_member_pointer<_Callable>,
                    std::is_invocable_r<_Result, decltype(std::declval<_Callable>()), _Type, _Args...>
                >, bool
            >
            = true
        >
        [[nodiscard]] list select(_Result match, _Callable member, const _Args&... args)
        {
            list sublist = list();
            for (_Type elem : *this)
            {
                if (std::invoke(member, elem, args...) == match)
                    sublist.push_back(elem);
            }
            return sublist;
        }


        ////////////////////
        /// TRANSFORMING ///
        ////////////////////

        template <
            typename _Result,
            typename _Transformer,
            typename... _Args,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_member_pointer<_Transformer>>,
                    std::is_invocable_r<_Result, decltype(std::declval<_Transformer>()), _Type, _Args...>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] list<_Result> transform(_Transformer transformer, _Args... args)
        {
            list<_Result> result = list<_Result>();
            for (_Type elem : *this)
                result.push_back(transformer(elem, args...));
            return result;
        }

        template <
            typename _Result,
            typename _Callable,
            typename... _Args,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_member_pointer<_Callable>,
                   std::is_invocable_r<_Result, decltype(std::declval<_Callable>()), _Type, _Args...>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] list<_Result> transform(_Callable member, _Args... args)
        {
            static auto transformer = [member](_Type obj, _Args... args)->_Result {
                return std::invoke(member, obj, args...);
            };
            list<_Result> result = list<_Result>();
            for (_Type elem : *this)
                result.push_back(transformer(elem, args...));
            return result;
        }

    };
}