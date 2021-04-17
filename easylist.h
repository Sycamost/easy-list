#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <sstream>

#include "convstr.h"

#define EASYLIST_TEMPLATE_OBJ_LIST_DEFN template<class _Type, class _Alloc = std::allocator<_Type>>
#define EASYLIST_TEMPLATE_OBJ_LIST_DECL template<class _Type, class _Alloc>
/*
namespace easylist
{
    EASYLIST_TEMPLATE_OBJ_LIST_DEFN
    class object_list : public std::vector<_Type, _Alloc>
    {
    public:
        using _Mybase = std::vector<_Type>;
        using allocator_type = typename _Mybase::allocator_type;
        using size_type = typename _Mybase::size_type;
        using difference_type = typename _Mybase::difference_type;
        using pointer = typename _Mybase::pointer;
        using const_pointer = typename _Mybase::const_pointer;
        using iterator = typename _Mybase::iterator;
        using const_iterator = typename _Mybase::const_iterator;
        using reverse_iterator = typename _Mybase::reverse_iterator;
        using const_reverse_iterator = typename _Mybase::const_reverse_iterator;

        object_list() : _Mybase() {}

        explicit object_list(const _Alloc& _Al) : _Mybase(_Al) {}

        object_list(const _Mybase& _Right, const _Alloc& _Al = _Alloc()) : _Mybase(_Right, _Al) {}

        object_list(const size_t _Count, const _Type& _Val, const _Alloc& _Al = _Alloc()) : _Mybase(_Count, _Val, _Al) {}

        template <class _Iter>
        object_list(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc()) : _Mybase(_First, _Last, _Al) {}

        object_list& operator=(const object_list& _Right) {
            return _Mybase::operator=((const _Mybase&)_Right);
        }

        object_list(object_list&& _Right, const _Alloc& _Al = _Alloc()) : _Mybase((_Mybase&&)_Right, _Al) {}

        object_list(_Mybase&& _Right, const _Alloc& _Al = _Alloc()) noexcept : _Mybase(_Right, _Al) {}

        object_list(std::initializer_list<_Type> _Ilist, const _Alloc& _Al = _Alloc()) : _Mybase(_Ilist, _Al) {}

        object_list(const object_list& _Right, const _Alloc& _Al = _Alloc()) : _Mybase((const _Mybase&)_Right, _Al) {}

        object_list& operator=(object_list&& _Right) {
            return _Mybase::operator=((_Mybase&&)_Right);
        }

        ~object_list() { }

    public:

        template <
            std::enable_if_t<std::is_convertible_v<_Type, std::string>, bool> = true
        >
        operator std::string() const
        {
            std::string str = "(";
            for (_Type elem : *this)
                str += (std::string)elem;
            str += ")";
            return str;
        }

        template <
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Type, std::string>,
                    std::is_convertible<_Type, char*>
                >,
                bool
            > = true
        >
        operator std::string() const
        {
            std::string str = "(";
            for (_Type elem : *this)
                str += std::string((char*)elem);
            str += ")";
            return str;
        }

    public:

        template <
            std::enable_if_t<
                std::conjunction_v<
                    std::is_convertible<_Type, std::string>,
                    std::is_convertible<_Type, char*>,
                    std::is_invocable<t2str, _Type>
                >
            >
        >
        operator std::string() const
        {
            std::string str = "(";
            for (_Type elem : *this)
                str += std::to_string(_Type);
            str += ")";
            return str;
        }

        operator const char* () const
        {
            return this->operator std::string().c_str;
        }

        friend std::ostream& operator<<(std::ostream& output, const object_list& list)
        {
            std::string str = list;
            output << str;
            return output;
        }
    };
}*/