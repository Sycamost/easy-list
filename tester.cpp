// easylist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "easylist.h"
#include "is_comparison.h"

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
    bool operator<(const C& rhs) const { return n < rhs.n; }
    bool operator>(const C& rhs) const { return n > rhs.n; }
};

bool isC1(C c) { return c.n == 1; }

int main()
{
    C c = C(1);

    object_list<C> cList = object_list<C>({ c, C(2), C(3) });

    static_assert(is_comparison_v<decltype(&C::operator<), C, C>);
    static_assert(is_comparison_v<decltype(&C::operator>), C, C>);
        
    return 0;
}
