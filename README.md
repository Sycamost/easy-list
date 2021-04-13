# easy-list
Makes C++ classes storing a list of either objects or n-tuples, with streamlined functionality for [searching](#Searching), [sorting](#Sorting), [iterating](#Iterating), [selecting](#Selecting) and [transforming](#Transforming).

<code>object_list</code> stores a list of classes or structs of a given type.

<code>tuple_list</code> is a specialisation of <code>object_list<tuple></code>.

Searching
---------

<code>object_list</code> can be searched for a match to a specified predicate, or a specified member (variable or method). This process is streamlined by use of a preprocessor macro. The following two expressions are equivalent:

    SEARCH_OBJ_LIST(list, member, value)
    list.search([term](ObjectType obj) -> bool { return obj.member == value; })

<code>tuple_list</code> can also be searched as above, but can also be easily searched for a match by tuple index. The following two expressions are equivalent:

    list.search(index, value)
    list.search([index, value](tuple t) -> bool { return t[index] == value; })
  
Sorting
-------

<code>object_list</code> can easily be sorted with a comparison function on the whole struct, or, by use of a preprocessor macro, a comparison function on a specified member (variable or method). The two statements following are equivalent:

    SORT_OBJ_LIST(list, member, comp);
    list.sort([comp](const ObjectType& obj1, const ObjectType& obj2) -> bool { return comp(obj1.member, obj2.member); });

<code>tuple_list</code> can be sorted as above, and also by a comparison function on a specified tuple index. The following two statements are equivalent:

    list.sort(index, comp);
    list.sort([index, comp](const tuple& t1, const tuple& t2) -> bool { return comp(t1[index], t2[index]); });

Iterating
---------

Both <code>object_list</code> and <code>tuple_list</code> can be iterated using the <code>for ( : )</code> structure.

Selecting
---------

A sub-list of <code>object_list</code> can easily be selected by predicate:

    list.select([](ObjectType obj) -> bool { return foo(obj); });

or by matching a specified value:

    list.select(obj)

or by matching a specified value on a member, by use of a pre-processor macro:

    SELECT_OBJ_LIST(list, member, value)
   
In addition to the above, a sub-list of a <code>tuple_list</code> can be selected by matching a value on a specified tuple index:

    list.select(index, value)

Transforming
------------

An <code>object_list</code> can be easily transformed into another <code>object_list</code> (possibly a different type) using the following intuitive syntax:

    list.transform([](SourceType obj) -> DestType { return foo(obj); })

Use of a pre-processor macro makes it easy to transform each element to a member of the source type. The following two expressions are equivalent:

    TRANSFORM_OBJ_LIST(list, member)
    list.transform([](SourceType obj) -> DestType { return obj.member; })
    
Also streamlined is the common operation of transforming a list of a one type into a list of another type to which the first is castable. The following two expressions are equivalent:

    list.transform<DestType>()
    list.transform([](SourceType obj) -> DestType { return (DestType)obj; })

As well as the above, <code>tuple_list</code> can be transformed to a specified tuple index. The following two expressions are equivalent:

    list.transform(index)
    list.transform([](tuple t) -> auto { return t[index]; })
