# easy-list
Makes C++ classes storing a list of either objects (classes or structs) or n-tuples, with streamlined functionality for [searching](#Searching), [sorting](#Sorting), [iterating](#Iterating), [selecting](#Selecting) and [transforming](#Transforming).

<code>object_list</code> stores a list of classes or structs of a given type.

<code>tuple_list</code> is a specialisation of <code>object_list<tuple></code>.

Searching
---------

<code>object_list</code> can be searched for a match to a specified predicate, or a specified member (variable or method). The following two expressions are equivalent:

    list.search(match, &ObjectType::memberVariable)
    list.search([match](ObjectType obj) -> bool { return obj.memberVariable == match; })

As are the following two expressions:

    ist.search(match, &ObjectType::getValue, args...)
    list.search([match, args](ObjectType obj) -> bool { return obj.getValue(args...) == match; })

<code>tuple_list</code> can also be searched as above, but can also be easily searched for a match by tuple index. The following two expressions are equivalent:

    list.search(index, value)
    list.search([index, value](tuple t) -> bool { return t[index] == value; })
  
Sorting
-------

<code>object_list</code> can easily be sorted with a comparison function on the whole object, or a comparison function on a specified member (variable or method). The two statements following are equivalent:

    list.sort(comp, &ObjectType::memberVariable)
    list.sort([comp](const ObjectType& obj1, const ObjectType& obj2) -> bool { return comp(obj1.memberVariable, obj2.memberVariable); });

As are the following two:

    list.sort(comp, &ObjectType::getValue, args...)
    list.sort([comp](const ObjectType& obj1, const ObjectType& obj2) -> bool { return comp(obj1.getValue(args...), obj2.getValue(args...); });

<code>tuple_list</code> can be sorted as above, and also by a comparison function on a specified tuple index. The following two statements are equivalent:

    list.sort(comp, index);
    list.sort([comp, index](const tuple& t1, const tuple& t2) -> bool { return comp(t1[index], t2[index]); });

Iterating
---------

Both <code>object_list</code> and <code>tuple_list</code> can be iterated using the <code>for ( : )</code> structure.

Selecting
---------

A sub-list of <code>object_list</code> can easily be selected by predicate:

    list.select([](ObjectType obj) -> bool { return foo(obj); });

or by matching a specified value:

    list.select(obj)

or by matching a specified value on a member:

    list.select(match, &ObjectType::memberVariable)
    list.select(match, &ObjectType::getValue, args...)
   
In addition to the above, a sub-list of a <code>tuple_list</code> can be selected by matching a value on a specified tuple index:

    list.select(index, value)

Transforming
------------

An <code>object_list</code> can be easily transformed into another <code>object_list</code> (possibly a different type) using the following intuitive syntax:

    list.transform([](SourceType obj) -> auto { return foo(obj); })

One may also transform each element to a member of the source type. The following two expressions are equivalent:

    list.transform(&SourceType::memberVariable)
    list.transform([](SourceType obj) -> auto { return obj.memberVariable; })

As are the following two:

    list.transform(&SourceType::getValue, args...)
    list.transform([](SourceType obj) -> auto { return obj.getValue(args...); })
    
Also streamlined is the common operation of transforming a list of a one type into a list of another type to which the first is castable. The following two expressions are equivalent:

    list.transform<DestType>()
    list.transform([](SourceType obj) -> DestType { return static_cast<DestType>(obj); })

As well as the above, <code>tuple_list</code> can be transformed to a specified tuple index. The following two expressions are equivalent:

    list.transform(index)
    list.transform([](tuple t) -> auto { return t[index]; })
