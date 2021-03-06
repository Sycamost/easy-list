#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <time.h>
#include <random>
#include <template_helpers.h>

#if _HAS_CXX17

namespace easy_list
{
    /// <summary>
    /// An ordered list with better built-in functionality than ordinary vectors.
    /// </summary>
    /// <typeparam name="_Type">The type of the elements of the list</typeparam>
    /// <typeparam name="_Alloc">The element allocator</typeparam>
    template<class _Type, class _Alloc = std::allocator<_Type>>
    class list : public std::vector<_Type, _Alloc>
    {
    public:
        using _Mybase = std::vector<_Type>;
        template <typename _Traits> using _String = std::basic_string<_Type, _Traits, _Alloc>;
        using value_type = _Type;
        using allocator_type = typename _Mybase::allocator_type;
        using size_type = typename _Mybase::size_type;

        /// <summary>
        /// Search operations return this if no match was found.
        /// </summary>
        /// <returns>An iterator object representing "no match found".</returns>
        typename _Mybase::iterator npos() { return this->end(); }

        /// <summary>
        /// Const search operations return this if no match was found.
        /// </summary>
        /// <returns>An iterator object representing "no match found".</returns>
        typename _Mybase::const_iterator npos() const { return this->end(); }

        ////////////////////
        /// CONSTRUCTORS ///
        ////////////////////

        list() : _Mybase() {}

        explicit list(const _Alloc& _Al) : _Mybase(_Al) {}

        list(const size_t _Count, const _Type& _Val, const _Alloc& _Al = _Alloc()) : _Mybase(_Count, _Val, _Al) {}

        template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        list(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc()) : _Mybase(_First, _Last, _Al) {}

        list(std::initializer_list<_Type> _Ilist, const _Alloc& _Al = _Alloc()) : _Mybase(_Ilist, _Al) {}

        list(const list& _Right, const _Alloc& _Al = _Alloc()) : _Mybase((const _Mybase&)_Right, _Al) {}
        list(const _Mybase& _Right, const _Alloc& _Al = _Alloc()) : _Mybase(_Right, _Al) {}
        template<typename _Traits> list(const _String<_Traits>& _Right, const _Alloc& _Al = _Alloc()) : _Mybase(_Right.begin(), _Right.end(), _Al) {}

        list(list&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase((_Mybase&&)_Right, _Al) {}
        list(_Mybase&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase(_Right, _Al) {}
        template<typename _Traits> list(_String<_Traits>&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase(_Right.begin(), _Right.end(), _Al) {}

        ~list() { }


        //////////////////////////
        /// OPERATOR OVERLOADS ///
        //////////////////////////

        list& operator=(const list& rhs) {
            _Mybase::operator=((const _Mybase&)rhs);
            return *this;
        }
        list& operator=(const _Mybase& rhs) {
            _Mybase::operator=(rhs);
            return *this;
        }
        template<typename _Traits>
        list& operator=(const _String<_Traits>& rhs) {
            return operator=(list(rhs));
        }

        list& operator=(list&& rhs) noexcept {
            _Mybase::operator=((_Mybase&&)rhs);
            return *this;
        }
        list& operator=(_Mybase&& rhs) noexcept {
            _Mybase::operator=(rhs);
            return *this;
        }
        template<typename _Traits>
        list& operator=(_String<_Traits>&& rhs) {
            return operator=(list(rhs));
        }

        /// <summary>
        /// Concatenates two lists, or a list and a vector.
        /// </summary>
        /// <param name="rhs">The other vector to concatenate.</param>
        /// <returns>The concatenated list.</returns>
        list operator+(const _Mybase& rhs) const {
            list result = list(*this);
            for (_Type elem : rhs) result.push_back(elem);
            return result;
        }

        /// <summary>
        /// Concatenates this list with a single item.
        /// </summary>
        /// <param name="rhs">The item to concatenate.</param>
        /// <returns>The concatenated list.</returns>
        list operator+(const _Type& rhs) const {
            auto result = list(*this);
            result.push_back(rhs);
            return result;
        }

        /// <summary>
        /// Appends (concatenates) a list or vector on the end of the current list.
        /// </summary>
        /// <param name="rhs">The vector to append.</param>
        /// <returns>The list, after the append operation.</returns>
        list& operator+=(const _Mybase& rhs) {
            this->insert(this->end(), rhs.begin(), rhs.end());
            return *this;
        }

        /// <summary>
        /// Appends (concatenates) a single item on the end of the current list.
        /// </summary>
        /// <param name="rhs">The item to append.</param>
        /// <returns>This list, after the append operation.</returns>
        list& operator+=(const _Type& rhs) {
            this->push_back(rhs);
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

        template <typename = typename std::enable_if_t<template_helpers::can_convert_wstring<_Type>::value, bool>>
        operator std::wstring() const
        {
            std::string typeName = getTypeName();
            std::wstring str = std::wstring(typeName.begin(), typeName.end());
            str += L": (";
            if (_Mybase::size() != 0)
            {
                for (_Type elem : *this)
                    str += template_helpers::convert_wstring(elem) + L", ";
                str = str.substr(0, str.length() - 2);
            }
            str += L")";
            return str;
        }

        friend std::ostream& operator<<(std::ostream& stream, const list list)
        {
            std::string str = list;
            stream << str;
            return stream;
        }

        friend std::wostream& operator<<(std::wostream& stream, const list list)
        {
            std::wstring wstr = list;
            stream << wstr;
            return stream;
        }


        ///////////////////
        /// CONVERSIONS ///
        ///////////////////

        std::basic_string<_Type, std::char_traits<_Type>, _Alloc> toString() const
        {
            return std::basic_string<_Type, std::char_traits<_Type>, _Alloc>(this->begin(), this->end());
        }


        /////////////////
        /// SEARCHING ///
        /////////////////

        /// <summary>
        /// Searches for an exact match.
        /// </summary>
        /// <typeparam name="_MatchType">A type equatable to the type of the elements of this list.</typeparam>
        /// <param name="match">The element to search for.</param>
        /// <returns>A const iterator to the first element found equal to the provided match.</returns>
        template <
            typename _MatchType,
            std::enable_if_t<
                template_helpers::is_equatable_v<const _Type&, const _MatchType&>,
                bool
            > = true
        >
        [[nodiscard]] typename _Mybase::const_iterator search(const _MatchType& match) const
        {
            return std::find_if(
                this->begin(),
                this->end(),
                [match](const _Type& other) -> bool { return other == match; }
            );
        }
        
        /// <summary>
        /// Searches for an element satisfying the given predicate.
        /// </summary>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>A const iterator to the first element found to satisfy the given predicate.</returns>
        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
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
        
        /// <summary>
        /// Searches for an element with a member matching the given value.
        /// </summary>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="match">The value to check against.</param>
        /// <param name="member">A reference to the member variable or method to check, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>A const iterator to the first element found, such that the given member variable or method returned the match provided.</returns>
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

        /// <summary>
        /// Checks whether the given element is contained in the list.
        /// </summary>
        /// <typeparam name="_MatchType">A type equatable to the type of the elements of this list.</typeparam>
        /// <param name="match">The element to search for.</param>
        /// <returns>True if a match was found, false otherwise.</returns>
        template <
            typename _MatchType,
            std::enable_if_t<
                template_helpers::is_equatable_v<const _Type&, const _MatchType&>,
                bool
            >
            = true
        >
        [[nodiscard]] bool contains(const _MatchType& match) const
        {
            return this->search(match) != this->npos();
        }

        /// <summary>
        /// Checks whether any element in the list satisfies the given predicate.
        /// </summary>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>True if any element satisfies the given predicate, false otherwise.</returns>
        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
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

        /// <summary>
        /// Checks whether any element in the list has the given member returning a match to the given value.
        /// </summary>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="match">The value to check against.</param>
        /// <param name="member">A reference to the member variable or method to check, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>True if any element in the list returned the given value on the given member, false otherwise.</returns>
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


        ///////////////////////
        /// UNIFY & DISJOIN ///
        ///////////////////////

        /// <summary>
        /// Makes a new list from the union of the elements of this list with the list or vector provided.
        /// </summary>
        /// <param name="rhs">The list or vector to unify with this one.</param>
        /// <returns>A list containing one instance of every element occurring at least once in either list, in the order in which they first occur in the right-hand side, followed by the order in which they first occur in this list.</returns>
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

        /// <summary>
        /// Makes a new list from the disjoint of the elements of this list with the list or vector provided.
        /// </summary>
        /// <param name="rhs">The list or vector to disjoin with this one.</param>
        /// <returns>A list containing one instance of every element occurring at least once in both lists, in the order in which they first occur in the right-hand side.</returns>
        [[nodiscard]] list disjoin(const _Mybase rhs) const
        {
            list<_Type, _Alloc> result = list<_Type, _Alloc>();
            for (_Type elem : rhs)
            {
                if (!result.contains(elem) && this->contains(elem))
                    result.push_back(elem);
            }
            return result;
        }

        /// <summary>
        /// Returns this list with all duplicates removed.
        /// </summary>
        [[nodiscard]] list removeDuplicates() const
        {
            return this->disjoin(*this);
        }

        /// <summary>
        /// Checks whether the two lists share any elements.
        /// </summary>
        /// <param name="rhs">The other list or vector.</param>
        /// <returns>True if any element was contained in both, false otherwise.</returns>
        bool shares(const _Mybase rhs) const
        {
            return this->disjoin(rhs).size() > 0;
        }


        ///////////////
        /// SORTING ///
        ///////////////

        /// <summary>
        /// Sorts the list with the given comparison.
        /// </summary>
        /// <typeparam name="_Compare">A comparison object type.</typeparam>
        /// <param name="comparer">The comparison to sort by.</param>
        /// <returns>This list, after having been sorted.</returns>
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

        /// <summary>
        /// Sorts the list with the default comparison.
        /// </summary>
        /// <returns>This list, after having been sorted.</returns>
        template <typename _Compare, std::enable_if_t<template_helpers::is_comparison_v<_Compare, _Type>, bool> = true>
        list& sort()
        {
            this->sort(std::less<>{});
            return *this;
        }

        /// <summary>
        /// Sorts the list with the given comparison on a specified member.
        /// </summary>
        /// <typeparam name="_Compare">A comparison object type on the type of the specified member variable or the return type of the member method, as applicable.</typeparam>
        /// <param name="comparer">The comparison to sort by.</param>
        /// <param name="member">A reference to the member variable or method to sort on, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>This list, after having been sorted.</returns>
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

        /// <summary>
        /// Sorts the list with the default comparison on a specified member.
        /// </summary>
        /// <param name="member">A reference to the member variable or method to sort on, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>This list, after having been sorted.</returns>
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

        /// <summary>
        /// Selects a sub-list containing all elements of this list equal to the provided match.
        /// </summary>
        /// <typeparam name="_MatchType">A type equatable to the type of the elements of this list.</typeparam>
        /// <param name="match">The element to search for.</param>
        /// <returns>A sub-list containing all elements of this list equal to the provided match.</returns>
        template <
            typename _MatchType,
            std::enable_if_t<
                template_helpers::is_equatable_v<_Type, _MatchType>,
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

        /// <summary>
        /// Selects a sub-list containing all elements of this list equal to the provided match.
        /// </summary>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>A sub-list containing all elements of this list satisfying the given predicate.</returns>
        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
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

        /// <summary>
        /// Selects a sub-list containing all elements of this list where the given member equals the provided match
        /// </summary>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="match">The value to match.</param>
        /// <param name="member">A reference to the member variable or method to check, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>A sub-list containing all elements of this list where the given member equals the provided match.</returns>
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
        /// REMOVING ///
        ////////////////

        /// <summary>
        /// Removes the element at the given index (if any element exists there) and returns the result.
        /// </summary>
        /// <param name="index">The index to remove at.</param>
        /// <returns>The result of the remove operation.</returns>
        [[nodiscard]] list removeAt(const long index) const
        {
            if (index < 0)
            {
                long newIndex = this->size() + index;
                if (newIndex < 0)
                    return *this;
                return this->removeAt(newIndex);
            }
            return this->slice(0, index) + this->slice(index + 1);
        }

        /// <summary>
        /// Removes the element at the given iterator (if any element exists there) and returns the result.
        /// </summary>
        /// <param name="index">The index to remove at.</param>
        /// <returns>The result of the remove operation.</returns>
        [[nodiscard]] list removeAt(const typename _Mybase::const_iterator iter) const
        {
            return this->removeAt(std::distance(this->begin(), iter));
        }

        /// <summary>
        /// Selects a sub-list containing all elements of this list not equal to the provided match.
        /// </summary>
        /// <typeparam name="_MatchType">A type equatable to the type of the elements of this list.</typeparam>
        /// <param name="match">The element to remove.</param>
        /// <returns>A sub-list containing all elements of this list not equal to the provided match.</returns>
        template <
            typename _MatchType,
            std::enable_if_t<
                template_helpers::is_equatable_v<_Type, _MatchType>,
                bool
            >
            = true
        >
        [[nodiscard]] list removeAll(const _MatchType& match) const
        {
            return this->select([match](_Type other) -> bool { return other != match; });
        }

        /// <summary>
        /// Removes the first match and returns the result.
        /// </summary>
        /// <typeparam name="_MatchType">A type equatable to the type of the elements of this list.</typeparam>
        /// <param name="match">The element to remove.</param>
        /// <returns>A sub-list of this with the first occurrence of the match removed.</returns>
        template <
            typename _MatchType,
            std::enable_if_t<
                template_helpers::is_equatable_v<_Type, _MatchType>,
                bool
            >
            = true
        >
            [[nodiscard]] list removeFirst(const _MatchType& match) const
        {
            return this->removeAt(this->search(match));
        }

        /// <summary>
        /// Selects a sub-list containing all elements of this list failing the given predicate.
        /// </summary>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>A sub-list containing all elements of this list failing the given predicate.</returns>
        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] list removeAll(const _Predicate predicate) const
        {
            return this->select([predicate](_Type other) -> bool { return !predicate(other); });
        }

        /// <summary>
        /// Removes the first element found satisfying the given predicate and returns the result.
        /// </summary>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>A sub-list of this with the first element satisfying the predicate removed.</returns>
        template <
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            >
            = true
        >
        [[nodiscard]] list removeFirst(const _Predicate predicate) const
        {
            return this->removeAt(this->search(predicate));
        }

        /// <summary>
        /// Selects a sub-list containing all elements of this list where the given member doesn't equal the provided match
        /// </summary>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="match">The value to (not) match.</param>
        /// <param name="member">A reference to the member variable or method to check, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>A sub-list containing all elements of this list where the given member doesn't equal the provided match.</returns>
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
        [[nodiscard]] list removeAll(const _Result& match, const _Callable member, const _Args&... args) const
        {
            return this->select([match, member, args...](_Type other) -> bool { return std::invoke(member, other, args...) != match; });
        }

        /// <summary>
        /// Removes the first element found whose specified members equals the specified match, and returns the result.
        /// </summary>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="match">The value to match.</param>
        /// <param name="member">A reference to the member variable or method to check, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>A sub-list of this with the first element where the given member equals the given match removed.</returns>
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
        [[nodiscard]] list removeFirst(const _Result& match, const _Callable member, const _Args&... args) const
        {
            return this->removeAt(this->search(match, member, args...));
        }


        ////////////////
        /// COUNTING ///
        ////////////////

        /// <summary>
        /// Finds the number of elements matching the specified value.
        /// </summary>
        /// <typeparam name="_MatchType">A type equatable to the type of the elements of this list.</typeparam>
        /// <param name="match">The element to search for.</param>
        /// <returns>The number of elements matching the specified value.</returns>
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

        /// <summary>
        /// Counts the number of elements satisfying the given predicate.
        /// </summary>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>The number of elements satisfying the given predicate.</returns>
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

        /// <summary>
        /// Counts the number of elements matching the given value on the given member.
        /// </summary>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="match">The value to match.</param>
        /// <param name="member">A reference to the member variable or method to match on, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>The number of elements matching the given value on the given member.</returns>
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

        /// <summary>
        /// Makes a new list based on this list, converting each element into the given type.
        /// </summary>
        /// <typeparam name="_ConvertibleType">A type to which the elements of this list are convertible.</typeparam>
        /// <returns>The result of the transformation.</returns>
        template<class _ConvertibleType, std::enable_if_t<std::is_convertible_v<_Type, _ConvertibleType>, bool> = true>
        [[nodiscard]] list<_ConvertibleType> transform()
        {
            auto result = list<_ConvertibleType>();
            for (_Type elem : *this)
                result.push_back(static_cast<_ConvertibleType>(elem));
            return result;
        }

        /// <summary>
        /// Makes a new list based on this list, converting each element to a new element by use of the given transformer.
        /// </summary>
        /// <typeparam name="_Result">The elements of the result of the transformation.</typeparam>
        /// <typeparam name="_Transformer">A callable object type, which takes elements of this list as arguments and returns elements of the result of the transformation.</typeparam>
        /// <param name="transformer">The transformation to apply to each element in this current list.</param>
        /// <param name="...args">The arguments to supply to the transformer, if applicable.</param>
        /// <returns>The result of the transformation.</returns>
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

        /// <summary>
        /// Makes a new list based on this list, converting each element into one of its members.
        /// </summary>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="member">A reference to the member variable or method to transform to, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>The result of the transformation.</returns>
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


        ///////////////
        /// REPLACE ///
        ///////////////

        /// <summary>
        /// Makes a new list from this one by replacing each match found with the given replacement value.
        /// </summary>
        /// <typeparam name="_Replacer">A type convertible to the elements of this list.</typeparam>
        /// <typeparam name="_MatchType">A type equatable to elements of this list.</typeparam>
        /// <param name="replacement">The value with which to replace the matched elements.</param>
        /// <param name="match">The value to match.</param>
        /// <returns>The result of the replacement.</returns>
        template <
            typename _Replacer,
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Replacer, _Type>,
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

        /// <summary>
        /// Makes a new list from this one by replacing each element satisfying the given predicate with the given replacement value.
        /// </summary>
        /// <typeparam name="_Replacer">A type convertible to the elements of this list.</typeparam>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="replacement">The value with which to replace the matched elements.</param>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>The result of the replacement.</returns>
        template <
            typename _Replacer,
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Replacer, _Type>,
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(_Replacer replacement, const _Predicate predicate) const
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

        /// <summary>
        /// Makes a new list from this one by replacing each element matching the specified value on the specified member with the specified value.
        /// </summary>
        /// <typeparam name="_Replacer">A type convertible to the elements of this list.</typeparam>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="replacement">The value with which to replace the matched elements.</param>
        /// <param name="match">The value to match.</param>
        /// <param name="member">A reference to the member variable or method to match on, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>The result of the replacement.</returns>
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
        [[nodiscard]] list replace(_Replacer replacement, const _Result& match, const _Callable member, const _Args&... args) const
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

        /// <summary>
        /// Makes a new list from this one by transforming each match found.
        /// </summary>
        /// <typeparam name="_Transformer">A callable object, taking elements of this list as its sole argument and returning another object of the same type.</typeparam>
        /// <typeparam name="_MatchType">A type equatable to elements of this list.</typeparam>
        /// <param name="transformer">The transformation to apply to each matched element.</param>
        /// <param name="match">The value to match.</param>
        /// <returns>The result of the replacement.</returns>
        template <
            typename _Transformer,
            typename _MatchType,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_convertible<_Transformer, _Type>>,
                    std::is_invocable_r<_Type, decltype(std::declval<_Transformer>()), _Type>,
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

        /// <summary>
        /// Makes a new list from this one by transforming each element satisfying the given predicate.
        /// </summary>
        /// <typeparam name="_Transformer">A callable object, taking elements of this list as its sole argument and returning another object of the same type.</typeparam>
        /// <typeparam name="_Predicate">A callable object, taking a element type as an argument and returning a bool.</typeparam>
        /// <param name="transformer">The transformation to apply to each matched element.</param>
        /// <param name="predicate">The predicate to check against.</param>
        /// <returns>The result of the replacement.</returns>
        template <
            typename _Transformer,
            typename _Predicate,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_convertible<_Transformer, _Type>>,
                    std::is_invocable_r<_Type, decltype(std::declval<_Transformer>()), _Type>,
                    std::negation<template_helpers::is_equatable<_Type, _Predicate>>,
                    template_helpers::is_predicate<_Predicate, _Type>
                >,
                bool
            > = true
        >
        [[nodiscard]] list replace(const _Transformer transformer, const _Predicate predicate) const
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

        /// <summary>
        /// Makes a new list from this one by transforming each element matching the specified value on the specified member.
        /// </summary>
        /// <typeparam name="_Transformer">A callable object, taking elements of this list as its sole argument and returning another object of the same type.</typeparam>
        /// <typeparam name="_Result">The type of the member variable, or return type of the member method, as applicable.</typeparam>
        /// <param name="transformer">The transformation to apply to each matched element.</param>
        /// <param name="match">The value to match.</param>
        /// <param name="member">A reference to the member variable or method to match on, as applicable.</param>
        /// <param name="...args">The arguments to pass to the member method, if applicable.</param>
        /// <returns>The result of the replacement.</returns>
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
        [[nodiscard]] list replace(const _Transformer transformer, const _Result& match, const _Callable member, const _Args&... args) const
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

        /////////////
        /// SLICE ///
        /////////////

        /// <summary>
        /// Makes a new list from a sub-string of elements of this one.
        /// </summary>
        /// <param name="start">The element to start at.</param>
        /// <param name="length">The length of the string (unless it goes beyond the end of this list, in which case we only go to the end of this list).</param>
        /// <returns>The result of the slice.</returns>
        list slice(const long start = LONG_MAX, const long length = LONG_MAX) const
        {
            if (start < 0)
            {
                long newStart = this->size() + start;
                if (newStart < 0)
                    return this->slice(0, length);
                return this->slice(newStart, length);
            }

            if (length < 0)
            {
                long newLength = this->size() + length - start;
                if (newLength < 0)
                    return this->slice(0, 0);
                return this->slice(start, newLength);
            }


            if (start >= this->size())
                return list();
            list result = list();
            size_t actualLength = (length > this->size() - start ? this->size() - start : length);
            result.reserve(actualLength);
            for (size_t i = 0; i < actualLength; i++)
                result.push_back((*this)[start + i]);
            return result;
        }

        /// <summary>
        /// Assigns to this list a sub-list formed of a sub-string of elements of the current list.
        /// </summary>
        /// <param name="start">The element to start at.</param>
        /// <param name="length">The length of the string (unless it goes beyond the end of this list, in which case we only go to the end of this list).</param>
        /// <returns>This list after splicing.</returns>
        list& splice(const size_t start, const long length = LONG_MAX)
        {
            if (length < 0)
            {
                long newLength = this->size() + length;
                if (newLength < 0)
                    return this->splice(start, 0);
                return this->splice(start, newLength);
            }
            *this = this->slice(start, length);
            return *this;
        }

        /// <summary>
        /// Returns a list of all unique substrings of this list of the given length.
        /// </summary>
        list<list> substrings(const size_t length) const
        {
            if (length < 0)
            {
                auto newLength = this->size() + length;
                if (newLength < 0)
                    return list<list>();
                return substrings(newLength);
            }

            if (length > this->size())
                return list<list>({ {} });

            list<list> result = list<list>();
            for (size_t i = 0; i <= this->size() - length; i++)
                result.push_back(this->slice(i, (long)length));

            // Remove duplicates before returning
            return result.removeDuplicates();
        }

        /// <summary>
        /// Returns a list of all unique substrings of this list.
        /// </summary>
        list<list> substrings() const
        {
            list<list> result = list<list>();
            for (size_t length = 0; length <= this->size(); length++)
                result += substrings(length);
            return result;
        }

        ///////////////
        /// SHUFFLE ///
        ///////////////

        /// <summary>
        /// Randomises the order of elements in this list.
        /// </summary>
        /// <returns>This list, after the shuffle.</returns>
        list& shuffle()
        {
            std::shuffle(this->begin(), this->end(), std::default_random_engine((unsigned int)time(NULL)));
            return *this;
        }


        ///////////////
        /// REVERSE ///
        ///////////////

        /// <summary>
        /// Reverses the order of elements in this list, and returns the result.
        /// </summary>
        [[nodiscard]] list reverse() const
        {
            list result = list();
            for (int i = (int)this->size() - 1; i >= 0; i--)
                result.push_back((*this)[i]);
            return result;
        }


        /////////////////
        /// POWER SET ///
        /////////////////

        list<list> powerSet() const
        {
            // Remove duplicates
            list set = this->removeDuplicates();

            // Start off with the empty set
            list<list> result = list<list>({ {} });

            // Generate the power set iteratively - on each iteration,
            // append a duplicate with the next element in this list prepended to each element in the result
            for (auto elem : set)
                result += result.transform<list>([elem](list li) -> list { return list({ elem }) + li; });

            return result;
        }


        ///////////////
        /// MIN/MAX ///
        ///////////////
        
        /// <summary>
        /// Finds the maximum under the given comparison.
        /// </summary>
        /// <typeparam name="_Compare">A comparison object type.</typeparam>
        /// <param name="comparer">The comparison function.</param>
        /// <returns>The greatest element in the list, if the comparison ordering is linear (otherwise, the return value is not defined).</returns>
        template <typename _Compare, std::enable_if_t<template_helpers::is_comparison_v<_Compare, _Type>, bool> = true>
        [[nodiscard]] _Type max(const _Compare comparer) const
        {
            return *std::max_element(this->begin(), this->end(), template_helpers::cast_static_comparison<_Compare, _Type>(comparer));
        }

        /// <summary>
        /// Finds the maximum under a default comparison.
        /// </summary>
        /// <returns>The greatest element in the list, if the comparison ordering is linear (otherwise, the return value is not defined).</returns>
        [[nodiscard]] _Type max() const
        {
            return *std::max_element(this->begin(), this->end());
        }

        /// <summary>
        /// Finds the minimum under the given comparison.
        /// </summary>
        /// <typeparam name="_Compare">A comparison object type.</typeparam>
        /// <param name="comparer">The comparison function.</param>
        /// <returns>The least element in the list, if the comparison ordering is linear (otherwise, the return value is not defined).</returns>
        template <typename _Compare, std::enable_if_t<template_helpers::is_comparison_v<_Compare, _Type>, bool> = true>
        [[nodiscard]] _Type min(const _Compare comparer) const
        {
            return *std::min_element(this->begin(), this->end(), template_helpers::cast_static_comparison<_Compare, _Type>(comparer));
        }

        /// <summary>
        /// Finds the minimum under a default comparison.
        /// </summary>
        /// <returns>The least element in the list, if the comparison ordering is linear (otherwise, the return value is not defined).</returns>
        [[nodiscard]] _Type min() const
        {
            return *std::min_element(this->begin(), this->end());
        }

    };
}

#else

#error "easy_list requires C++17 or greater."

#endif // _HAS_CXX_17