# easy-list
Makes a C++ class, <code>list</code> storing a list of arbitrary objects, with streamlined functionality for [searching](#Searching), [sorting](#Sorting), [iterating](#Iterating), [selecting](#Selecting), [counting](#Counting), [transforming](#Transforming) and [other stuff](#Other stuff). <code>list</code> publically inherits from <code>std::vector</code>, so any member functions of the latter may also be called on the former. Some vector functions have been overwritten, but the user can expect behaviour in such cases to imitate the behaviour of the original function.

Searching
---------

<code>list</code> can be searched for a match to a specified predicate, or a specified member (variable or method). Searching returns an iterator to the first found match, or <code>end()</code> if no match was found. The following two expressions are equivalent:

    myList.search(match, &ObjectType::memberVariable)
    myList.search([match](ObjectType obj) -> bool { return obj.memberVariable == match; })

As are the following two expressions:

    myList.search(match, &ObjectType::getValue, args...)
    myList.search([match, args](ObjectType obj) -> bool { return obj.getValue(args...) == match; })

If the user doesn't need to see the result, but just needs to know whether such an object is contained in the list at all, there is a <code>contains</code> method. The following two expressions are equivalent:

    myList.contains([*whatever*])
    (myList.search([*whatever*]) != myList.end())
  
Sorting
-------

<code>list</code> can easily be sorted with a comparison function on the whole object, or a comparison function on a specified member (variable or method). Sorting is applied to the object on which the sort was called, and also returns the result. The two statements following are equivalent:

    myList.sort(comp, &ObjectType::memberVariable)
    myList.sort([comp](const ObjectType& obj1, const ObjectType& obj2) -> bool { return comp(obj1.memberVariable, obj2.memberVariable); });

As are the following two:

    myList.sort(comp, &ObjectType::getValue, args...)
    myList.sort([comp](const ObjectType& obj1, const ObjectType& obj2) -> bool { return comp(obj1.getValue(args...), obj2.getValue(args...); });

Iterating
---------

<code>list</code> can be iterated using the <code>for ( : )</code> structure. It inherits this functionality from <code>std::vector</code>.

Selecting
---------

Selecting a <code>list</code> means creating a new <code>list</code> with a specified sub-class of the elements of the old <code>list</code>. Selection can be done by predicate:

    myList.select([](ObjectType obj) -> bool { return isFoo(obj); });

or by matching a specified value:

    myList.select(obj)

or by matching a specified value on a member:

    myList.select(match, &ObjectType::memberVariable)
    myList.select(match, &ObjectType::getValue, args...)

Counting
--------

Counting is simply a shortcut for selecting, then finding the size of the resulting selection. In any event, <code>myList.count([*whatever*])</code> is the same as <code>myList.select([*whatever*]).size()</code>.

Transforming
------------

Transforming means turning one <code>list</code> into another, by use of a transformer: a map from each element of the old <code>list</code> to its corresponding element in the new. A <code>list</code> can be easily transformed into another <code>list</code> (possibly a different type) using the following intuitive syntax:

    myList.transform([](ObjectType obj) -> auto { return foo(obj); })

If the source type is an object type, one may also transform each element to a member of the source type. The following two expressions are equivalent:

    myList.transform(&ObjectType::memberVariable)
    myList.transform([](ObjectType obj) -> auto { return obj.memberVariable; })

As are the following two:

    myList.transform(&ObjectType::getValue, args...)
    myList.transform([](ObjectType obj) -> auto { return obj.getValue(args...); })
    
Also streamlined is the common operation of transforming a list of a one type into a list of another type to which the first is castable. The following two expressions are equivalent:

    myList.transform<ConvertibleType>()
    myList.transform([](ObjectType obj) -> auto { return static_cast<ConvertibleType>(obj); })

Other stuff
-----------

You can create a new list from one list and a vector with <code>unify</code> (for lack of being able to use the keyword <code>union</code>), where the new list contains one of every unique element of each of the conjuncts, in the order they first appear in the left list, followed by the order they first appear in the right list.

Similarly, <code>disjoin<code> concatenates two vectors, and then removes all elements which fail to be in both disjoints. (As a result, repeats are allowed in disjoins, but not in unifications.)

You can check if two lists share any elements with <code>shares</code>, which is equivalent to disjoining the vectors, and checking to see if the result is non-empty.

You can concatenate two lists (or a list and a vector) with the + operator.