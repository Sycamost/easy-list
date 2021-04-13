# easy-list
Makes C++ classes storing a list of either objects or n-tuples, with streamlined functionality for [searching](#Searching), [sorting](#Sorting), iterating, transforming, selecting and mapping.

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


Transforming
------------


Selecting
---------


Mapping
-------
