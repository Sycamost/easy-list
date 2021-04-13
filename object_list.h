#pragma once
#include <vector>
#include <algorithm>

using namespace std;

template<class _Type, class _Alloc = allocator<_Type>>
class object_list : public vector<_Type>
{
public:
    using _Mybase = vector<_Type>;
    using value_compare = typename _Mybase::value_compare;
    using allocator_type = typename _Mybase::allocator_type;
    using size_type = typename _Mybase::size_type;
    using difference_type = typename _Mybase::difference_type;
    using pointer = typename _Mybase::pointer;
    using const_pointer = typename _Mybase::const_pointer;
    using iterator = typename _Mybase::iterator;
    using const_iterator = typename _Mybase::const_iterator;
    using reverse_iterator = typename _Mybase::reverse_iterator;
    using const_reverse_iterator = typename _Mybase::const_reverse_iterator;
    using _Alnode = typename _Mybase::_Alnode;
    using _Alnode_traits = typename _Mybase::_Alnode_traits;

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

    object_list& operator=(object_list&& _Right) {
        return _Mybase::operator=((_Mybase&&)_Right);
    }

    object_list(initializer_list<_Type> _Ilist, const _Alloc& _Al = _Alloc()) : _Mybase(_Ilist, _Al) {}

    object_list(const object_list& _Right, const _Alloc& _Al = _Alloc()) : _Mybase((const _Mybase&)_Right, _Al) {}

    object_list& operator=(object_list&& _Right) {
        return _Mybase::operator=((_Mybase&&)_Right);
    }

    ~object_list() : ~_Mybase() {}
};