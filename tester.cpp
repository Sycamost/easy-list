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
    bool operator<(const C& rhs) const { return n < rhs.n; }
    bool operator>(const C& rhs) const { return n > rhs.n; }
};

bool odder(const C& c1, const C& c2) { return !(c1.n % 2) && (c2.n % 2); }

bool evener(const C& c1, const C& c2) { return (c1.n % 2) && !(c2.n % 2); }

int main()
{
    C c = C(1);

    object_list<C> cList = object_list<C>({ c, C(2), C(3) });

    cList.sort();
    std::cout << cList << std::endl;

    cList.sort(&C::operator>);
    std::cout << cList << std::endl;

    cList.sort(&C::operator<);
    std::cout << cList << std::endl;

    cList.sort(&odder);
    std::cout << cList << std::endl;

    cList.sort(&evener);
    std::cout << cList << std::endl;
        
    return 0;
}
