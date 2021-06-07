# easy-list
Makes a C++ class, <code>list</code> storing a list of arbitrary objects, with streamlined functionality for

* [searching](#Searching),
* [sorting](#Sorting),
* [iterating](#Iterating),
* [selecting](#Selecting),
* [removing](#Removing),
* [counting](#Counting),
* [transforming](#Transforming),
* [slicing and splicing](#Slicing),
* [replacing](#Replacing)

and [other stuff](#Miscellany). <code>list</code> publically inherits from <code>std::vector</code>, so any member functions of the latter may also be called on the former. Some vector functions have been overwritten, but the user can expect behaviour in such cases to imitate the behaviour of the original function.

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

The user can select all *but* specified members by calling <code>remove</code>, with like syntax to the above.

Removing
--------

There are actually three kinds of removal operations:

* <code>removeAt</code> simply removes the element at the specified index or iterator.

* <code>removeFirst</code> is equivalent to finding an iterator with [<code>search</code>](#Searching) and calling <code>removeAt</code> with that iterator.

* <code>removeAll</code> is equivalent to [<code>select</code>](#Selecting) with the negation of the given search criteria.

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

Slicing
-------

Lists can be sliced. This means selecting a specified sub-string of elements from the original list, with the following syntax:

    myList.slice(start, length)
   
Where <code>start</code> is a <code>size_t</code> representing the first index of the sub-string and <code>length</code> is the length of the sub-string. To also assign the result to the list on which it was called, use <code>splice</code>.

You can also get a list of all possible sub-strings by calling <code>substrings()</code>, or of all possible sub-strings of a given length with <code>substrings(int length)</code>.

Replacing
---------

Lists can be replaced, meaning selected elements can be interchanged with specified replacements. Finding the elements to replace can be done by simple match, by member match or by predicate, and the replacement can be done by simple replacement or by a simple transformer (no arguments allowed). <code>replace</code> returns the result of this operation.

Miscellany
-----------

You can create a new list from one list and a vector with <code>unify</code> (for lack of being able to use the keyword <code>union</code>), where the new list contains exactly one of every element that occurs at least once in *either* of the two starting lists, in the order they first appear in the left list, followed by the order they first appear in the right list.

Similarly, <code>disjoin</code> returns a list containing exactly one instance of every element that occurs at least once in *both* lists, in the order in which they first appear in the right-hand list.

All duplicates can be removed from a list with <code>removeDuplicates()</code>. Behaviour is equivalent to disjoining a list with itself.

You can check if two lists share any elements with <code>shares</code>, which is equivalent to disjoining the vectors, and checking to see if the result is non-empty.

You can concatenate two lists (or a list and a vector) with the + operator. As a shorthand for concatenating a list with a single item, you can simply concatenate an item.

You can shuffle lists by simply calling <code>shuffle()</code>. No arguments required. For almost all purposes, you don't want to know anything about the RNG or the shuffle algorithm.

Anywhere where you pass an index to a new easy_list::list function, you can pass a negative value. Any negative value, -x, will be interpreted as the length of the list minus x.

You can reverse the order of elements in a list with <code>reverse()</code>.

You can turn any list into a list of the power set of the elements of that list with <code>powerSet()</code>.

You can find the min or max of a list, You may supply your own ordering function - if you don't, the < and > operators will be used if they are defined; and if not, they are ordered by hash code.