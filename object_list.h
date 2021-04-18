#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <template_helpers.h>
#include "easylist_util.h"


namespace easylist
{
    EASYLIST_TEMPLATE_OBJ_LIST_DEFN
        class object_list : public std::vector<_Type, _Alloc>
    {
    public:
        using _Mybase = std::vector<_Type>;
        using value_type = _Type;
        using allocator_type = typename _Mybase::allocator_type;
        using size_type = typename _Mybase::size_type;



        ////////////////////
        /// CONSTRUCTORS ///
        ////////////////////

        object_list() : _Mybase() {}

        explicit object_list(const _Alloc& _Al) : _Mybase(_Al) {}

        object_list(const _Mybase& _Right, const _Alloc& _Al = _Alloc()) : _Mybase(_Right, _Al) {}

        object_list(const size_t _Count, const _Type& _Val, const _Alloc& _Al = _Alloc()) : _Mybase(_Count, _Val, _Al) {}

        template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        object_list(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc()) : _Mybase(_First, _Last, _Al) {}

        object_list(object_list&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase((_Mybase&&)_Right, _Al) {}

        object_list(_Mybase&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase(_Right, _Al) {}

        object_list(std::initializer_list<_Type> _Ilist, const _Alloc& _Al = _Alloc()) : _Mybase(_Ilist, _Al) {}

        object_list(const object_list& _Right, const _Alloc& _Al = _Alloc()) : _Mybase((const _Mybase&)_Right, _Al) {}

        ~object_list() { }


        //////////////////////////
        /// OPERATOR OVERLOADS ///
        //////////////////////////

        object_list& operator=(const object_list& _Right) {
            return _Mybase::operator=((const _Mybase&)_Right);
        }

        object_list& operator=(object_list&& _Right) noexcept {
            _Mybase::operator=((_Mybase&&)_Right);
            return *this;
        }

        template <std::enable_if_t<can_convert_string_v<_Type>, bool> = true>
        operator std::string() const
        {

            std::string str = "object_list<";
            str += typeid(_Type).name();
            str += ">: (";
            if (_Mybase::size() != 0)
            {
                for (_Type elem : *this)
                    str += convert_string(elem) + ", ";
                str = str.substr(0, str.length() - 2);
            }
            str += ")";
            return str;
        }

        friend std::ostream& operator<<(std::ostream& output, const object_list& list)
        {
            std::string str = list;
            output << str;
            return output;
        }


        /////////////////
        /// SEARCHING ///
        /////////////////

        template <std::enable_if_t<is_equatable_self_v<_Type>, bool> = true>
        typename _Mybase::iterator search(const _Type match)
        {
            return std::find(this->begin(), this->end(), match);
        }

        template <
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _MatchType>>,
                    is_equatable<_Type, _MatchType>
                >,
                bool
            > = true
        >
        typename _Mybase::iterator search(const _MatchType match)
        {
            return std::find(this->begin(), this->end(), [match](_Type other) -> bool { return other == match; });
        }

        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _Predicate>>,
                    std::negation<is_equatable<_Type, _Predicate>>,
                    is_predicate<_Predicate, _Type>
                >,
                bool
            > = true
        >
        typename _Mybase::iterator search(_Predicate predicate)
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
            > = true
        >
        typename _Mybase::iterator search(_Result match, _Callable member, const _Args&... args)
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

        template <typename _Compare, std::enable_if_t<is_comparison_v<_Compare, _Type>, bool> = true>
        void sort(_Compare comparer)
        {
            std::sort(this->begin(), this->end(), cast_static_comparison<_Compare, _Type>(comparer));
        }

        void sort()
        {
            this->sort(std::less<>{});
        }

        template <
            typename _Compare,
            typename _Callable,
            typename... _Args,
            typename _Result = std::remove_reference_t<decltype(std::invoke(std::declval<_Callable>(), std::declval<_Type>(), std::declval<_Args>()...))>,
            std::enable_if_t<
                std::conjunction_v<
                    is_comparison<_Compare, _Result>,
                    is_const_member_of<_Callable, _Type, _Args...>
                >,
                bool
            > = true
        >
        void sort(_Compare comparer, _Callable member, const _Args&... args)
        {
            static auto static_comparer = cast_static_comparison<_Compare, _Type>(comparer);
            static auto pred = [comparer, member, args...](const _Type& lhs, const _Type& rhs) -> auto {
                const _Result resultLhs = std::invoke(member, lhs, args...);
                const _Result resultRhs = std::invoke(member, rhs, args...);
                return static_comparer(resultLhs, resultRhs);
            };
            std::sort(this->begin(), this->end(), pred);
        }

        template <
            typename _Callable,
            typename... _Args
        >
        void sort(_Callable member, const _Args&... args)
        {
            this->sort(std::less<>{}, member, args...);
        }


        ////////////////////
        /// TRANSFORMING ///
        ////////////////////

        template <
            typename _Transformer,
            typename... _Args,
            typename _Result,
            std::enable_if_t<
                std::is_invocable_r_v<_Result, _Transformer, _Args...>,
                bool
            > = true
        >
        object_list<_Result> transform(_Transformer transformer)
        {
            object_list<_Result> result = object_list<_Result>();
            for (_Type elem : *this)
            {
                result.push_back(transformer(result));
            }
            return result;
        }
    };
}