#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <template_helpers.h>

#if _HAS_CXX17

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

        typename _Mybase::iterator npos() { return this->end(); }
        typename _Mybase::const_iterator npos() const { return this->end(); }

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

        list& operator=(const list& rhs) {
            return _Mybase::operator=((const _Mybase&)rhs);
        }

        list& operator=(list&& rhs) noexcept {
            _Mybase::operator=((_Mybase&&)rhs);
            return *this;
        }

        list operator+(const _Mybase& rhs) const {
            list result = list(*this);
            result.insert(result.end(), rhs.begin(), rhs.end());
            return result;
        }

        list& operator+=(const _Mybase& rhs) {
            this->insert(this->end(), rhs.begin(), rhs.end());
            return *this;
        }

    private:
        std::string getTypeName() const
        {
            std::string str = "easy_list::list<";
            str += typeid(_Type).name();
            str += ">";
            return str;
        }

    public:
        template <typename = typename std::enable_if_t<template_helpers::can_convert_string<_Type>::value, bool>>
        operator std::string() const
        {

            std::string str = getTypeName();
            str += ": (";
            if (_Mybase::size() != 0)
            {
                for (_Type elem : *this)
                    str += template_helpers::convert_string(elem) + ", ";
                str = str.substr(0, str.length() - 2);
            }
            str += ")";
            return str;
        }

        friend std::ostream& operator<<(std::ostream& output, const list list)
        {
            std::string str = list;
            output << str;
            return output;
        }


        /////////////////
        /// SEARCHING ///
        /////////////////

        template <typename = typename std::enable_if_t<template_helpers::is_equatable_self_v<_Type>, bool>>
        [[nodiscard]] typename _Mybase::const_iterator search(const _Type& match) const
        {
            return std::find(this->begin(), this->end(), match);
        }

        template <
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _MatchType>>,
                    template_helpers::is_equatable<const _Type&, const _MatchType&>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] typename _Mybase::const_iterator search(const _MatchType& match) const
        {
            return std::find_if(
                this->begin(),
                this->end(),
                [match](const _Type& other) -> bool { return other == match; }
            );
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
        [[nodiscard]] typename _Mybase::const_iterator search(const _Predicate predicate) const
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
        [[nodiscard]] typename _Mybase::const_iterator search(const _Result& match, const _Callable member, const _Args&... args) const
        {
            return std::find_if(
                this->begin(),
                this->end(),
                [match, member, args...](const _Type& other)
                -> bool { return std::invoke(member, other, args...) == match; }
            );
        }


        ////////////////
        /// CONTAINS ///
        ////////////////

        template <typename = typename std::enable_if_t<template_helpers::is_equatable_self_v<_Type>, bool>>
        [[nodiscard]] bool contains(const _Type& match) const
        {
            return this->search(match) != this->npos();
        }

        template <
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<_Type, _MatchType>>,
                    template_helpers::is_equatable<const _Type&, const _MatchType&>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] bool contains(const _MatchType& match) const
        {
            return this->search(match) != this->npos();
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
        [[nodiscard]] bool contains(const _Predicate predicate) const
        {
            return this->search(predicate) != this->npos();
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
        [[nodiscard]] bool contains(const _Result& match, const _Callable member, const _Args&... args) const
        {
            return this->search(match, member, args...) != this->npos();
        }


        ///////////////
        /// SORTING ///
        ///////////////

        template <typename _Compare, std::enable_if_t<template_helpers::is_comparison_v<_Compare, _Type>, bool> = true>
        list& sort(const _Compare comparer)
        {
            std::sort(
                this->begin(),
                this->end(),
                template_helpers::cast_static_comparison<_Compare, _Type>(comparer)
            );
            return *this;
        }

        list& sort()
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
        list& sort(const _Compare comparer, const _Callable member, const _Args&... args)
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
        list& sort(const _Callable member, const _Args&... args)
        {
            this->sort(std::less<>{}, member, args...);
            return *this;
        }


        /////////////////
        /// SELECTING ///
        /////////////////

        template <typename = typename std::enable_if_t<template_helpers::is_equatable_self_v<_Type>, bool>>
        [[nodiscard]] list select(const _Type& match) const
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
        [[nodiscard]] list select(const _MatchType& match) const
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
        [[nodiscard]] list select(const _Predicate predicate) const
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
        [[nodiscard]] list select(const _Result& match, const _Callable member, const _Args&... args) const
        {
            list sublist = list();
            for (_Type elem : *this)
            {
                if (std::invoke(member, elem, args...) == match)
                    sublist.push_back(elem);
            }
            return sublist;
        }


        ////////////////
        /// COUNTING ///
        ////////////////

        template <typename = typename std::enable_if_t<template_helpers::is_equatable_self_v<_Type>, bool>>
        [[nodiscard]] size_t count(const _Type& match) const
        {
            return this->select(match).size();
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
        [[nodiscard]] size_t count(const _MatchType& match) const
        {
            return this->select(match).size();
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
        [[nodiscard]] size_t count(const _Predicate predicate) const
        {
            return this->select(predicate).size();
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
        [[nodiscard]] size_t count(const _Result& match, const _Callable member, const _Args&... args) const
        {
            return this->select(match, member, args...).size();
        }


        ////////////////////
        /// TRANSFORMING ///
        ////////////////////

        template<class _ConvertibleType, std::enable_if_t<std::is_convertible_v<_Type, _ConvertibleType>, bool> = true>
        [[nodiscard]] list<_ConvertibleType> transform()
        {
            auto result = list<_ConvertibleType>();
            for (_Type elem : *this)
                result.push_back(static_cast<_ConvertibleType>(elem));
            return result;
        }

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
        [[nodiscard]] list<_Result> transform(const _Transformer transformer, const _Args&... args) const
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
        [[nodiscard]] list<_Result> transform(const _Callable member, const _Args&... args) const
        {
            static auto transformer = [member](const _Type& obj, const _Args&... args)->_Result {
                return std::invoke(member, obj, args...);
            };
            list<_Result> result = list<_Result>();
            for (_Type elem : *this)
                result.push_back(transformer(elem, args...));
            return result;
        }

        /// Unify ///
        [[nodiscard]] list unify(const _Mybase rhs) const
        {
            list<_Type, _Alloc> result = list<_Type, _Alloc>();
            for (_Type elem : *this + rhs)
            {
                if (!result.contains(elem))
                    result.push_back(elem);
            }
            return result;
        }


        ///////////////
        /// REPLACE ///
        ///////////////

        /// With simple replacement (substitution) ///

        template <
            typename _Replacer,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Replacer, _Type>,
                    template_helpers::is_equatable_self<_Type>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(_Replacer replacement, const _Type& match) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (elem == match)
                    result.push_back(replacement);
                else
                    result.push_back(elem);
            }
            return result;
        }

        template <
            typename _Replacer,
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Replacer, _Type>,
                    std::negation<std::is_same<_Type, _MatchType>>,
                    template_helpers::is_equatable<_Type, _MatchType>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(_Replacer replacement, const _MatchType& match) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (elem == match)
                    result.push_back(replacement);
                else
                    result.push_back(elem);
            }
            return result;
        }

        template <
            typename _Replacer,
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Replacer, _Type>,
                    std::negation<std::is_same<_Type, _Predicate>>,
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(_Replacer replacement, const _Predicate& predicate) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (predicate(elem))
                    result.push_back(replacement);
                else
                    result.push_back(elem);
            }
            return result;
        }

        template <
            typename _Replacer,
            typename _Result,
            typename _Callable,
            typename... _Args,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Replacer, _Type>,
                    std::is_member_pointer<_Callable>,
                    std::is_invocable_r<_Result, decltype(std::declval<_Callable>()), _Type, _Args...>
                >, bool
            > = true
        >
        [[nodiscard]] list replace(_Replacer replacement, const _Result& match, const _Callable& member, const _Args&... args) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (std::invoke(member, elem, args...) == match)
                    result.push_back(replacement);
                else
                    result.push_back(elem);
            }
            return result;
        }

        /// With transformer ///

        template <
            typename _Transformer,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_convertible<_Transformer, _Type>>,
                    std::is_invocable_r<_Type, decltype(std::declval<_Transformer>()), _Type>,
                    template_helpers::is_equatable_self<_Type>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(const _Transformer transformer, const _Type& match) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (elem == match)
                    result.push_back(std::invoke(transformer, elem));
                else
                    result.push_back(elem);
            }
            return result;
        }

        template <
            typename _Transformer,
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_convertible<_Transformer, _Type>>,
                    std::is_invocable_r<_Type, decltype(std::declval<_Transformer>()), _Type>,
                    std::negation<std::is_same<_Type, _MatchType>>,
                    template_helpers::is_equatable<_Type, _MatchType>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(const _Transformer transformer, const _MatchType& match) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (elem == match)
                    result.push_back(std::invoke(transformer, elem));
                else
                    result.push_back(elem);
            }
            return result;
        }

        template <
            typename _Transformer,
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_convertible<_Transformer, _Type>>,
                    std::is_invocable_r<_Type, decltype(std::declval<_Transformer>()), _Type>,
                    std::negation<std::is_same<_Type, _Predicate>>,
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(const _Transformer transformer, const _Predicate& predicate) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (predicate(elem))
                    result.push_back(std::invoke(transformer, elem));
                else
                    result.push_back(elem);
            }
            return result;
        }

        template <
            typename _Transformer,
            typename _Result,
            typename _Callable,
            typename... _Args,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_convertible<_Transformer, _Type>>,
                    std::is_invocable_r<_Type, decltype(std::declval<_Transformer>()), _Type>,
                    std::is_member_pointer<_Callable>,
                    std::is_invocable_r<_Result, decltype(std::declval<_Callable>()), _Type, _Args...>
                >, bool
            > = true
        >
        [[nodiscard]] list replace(const _Transformer transformer, const _Result& match, const _Callable& member, const _Args&... args) const
        {
            list result = list();
            for (_Type elem : *this)
            {
                if (std::invoke(member, elem, args...) == match)
                    result.push_back(std::invoke(transformer, elem));
                else
                    result.push_back(elem);
            }
            return result;
        }

        /// Disjoin ///
        [[nodiscard]] list disjoin(const _Mybase rhs) const
        {
            list<_Type, _Alloc> result = list<_Type, _Alloc>();
            for (_Type elem : *this)
            {
                if (std::find(rhs.begin(), rhs.end(), elem) != rhs.end())
                    result.push_back(elem);
            }
            for (_Type elem : rhs)
            {
                if (this->contains(elem))
                    result.push_back(elem);
            }
            return result;
        }

        /// Shares ///
        bool shares(const _Mybase rhs) const
        {
            return this->disjoin(rhs).size() > 0;
        }

        /// Slice ///
        list slice(const size_t start, const size_t length = -1) const
        {
            if (start >= this->size())
                return list();
            list result = list();
            size_t actualLength = (length > this->size() - start ? this->size() - start : length);
            result.reserve(actualLength);
            for (size_t i = 0; i < actualLength; i++)
                result.push_back((*this)[start + i]);
            return result;
        }

        /// Splice ///
        list splice(const size_t start, const size_t length = -1)
        {
            *this = this->slice(start, length);
            return *this;
        }

    };
}

#else

#error "easy_list requires C++17 or greater."

#endif // _HAS_CXX_17