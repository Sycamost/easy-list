#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <sstream>
#include <iterator>
#include <algorithm>
#include "easylist_util.h"
#include "is_equatable.h"
#include "is_predicate.h"


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
            class _MatchType,
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
            class _Predicate,
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
    };
}