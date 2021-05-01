// easylist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "easy_list.h"

using namespace easy_list;

class C
{
public:
    int n;
    C(int n) { this->n = n; }
    int get() const { return n; }
    int getDiff(int other) const { return (n > other ? n - other : other - n); }
    operator const std::string() const { return "I am number " + std::to_string(n); }
    operator const std::wstring() const { return L"I am number " + std::to_wstring(n); }
    operator const int() const { return n; }
    friend std::ostream& operator<<(std::ostream& stream, const C& c) { return stream << (std::string)(c); }
    bool operator==(const C other) const { return n == other.n; }
    bool operator==(const int other) const { return n == other; }
    bool operator<(const C rhs) const { return n < rhs.n; }
    bool operator>(const C rhs) const { return n > rhs.n; }
};

bool odder(const C& c1, const C& c2) { return !(c1.n % 2) && (c2.n % 2); }

bool evener(const C& c1, const C& c2) { return (c1.n % 2) && !(c2.n % 2); }

bool isEven(const C& c) {
    return c.n % 2 == 0;
}

C plusN(const C& c, int n) { return C(c.get() + n); }
C plusOne(const C& c) { return plusN(c, 1); }

int main()
{
    const list<char> l1 = { 'H', 'e', 'l', 'l', 'o' };
    list<wchar_t> l2 = { L'w', L'o', L'r', L'l', L'd', L'!' };
    std::cout << l1.toString() << " ";
    std::wcout << l2.toString() << "\n";
    return 0;
}
