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
    int get() const { return n; }
    int getDiff(int other) const { return (n > other ? n - other : other - n); }
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
    const C c = C(1);
    c.getDiff(0);

    object_list<C> cList = object_list<C>({ C(0), C(2), C(-3) });

    cList.sort(std::less<>{}, & C::n);
    std::cout << cList << std::endl;

    cList.sort(std::less<>{}, & C::getDiff, 0);
    std::cout << cList << std::endl;

    cList.sort(& C::get);
    std::cout << cList << std::endl;
        
    return 0;
}
