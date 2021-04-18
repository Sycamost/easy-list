// easylist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "easylist.h"

using namespace easylist;

class C
{
public:
    int n;
    C(int n) { this->n = n; }
    int get() { return n; }
    operator const std::string() const { return "I am number " + std::to_string(n); }
    friend std::ostream& operator<<(std::ostream& stream, const C& c) { return stream << (std::string)(c); }
    bool operator==(const C& other) const { return n == other.n; }
};

bool isC1(C c) { return c.n == 1; }

int main()
{
    C c = C(1);

    object_list<C> cList = object_list<C>({ c, C(2), C(3) });
    std::cout << cList << std::endl;

    auto iter = cList.search(1, &C::n);
    if (iter != cList.end())
        std::cout << "A: " << *iter << std::endl;

    iter = cList.search(1, &C::get);
    if (iter != cList.end())
        std::cout << "B: " << *iter << std::endl;
        
    return 0;
}
