// easylist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include "easylist.h"

using namespace easylist;

class C
{
public:
    int n;
    C(int n) { this->n = n; }
    int get() { return n; }
    operator const std::string() const { return std::to_string(n) + " - Hello world!"; }
};

int main()
{
    double d1 = 2.2, d2 = 2.1, d3 = 2.0;

    object_list<int> intList = object_list<int>({ 42 });
    object_list<float> floatList = object_list<float>({ 1.2f, 1.1f, 1.0f });
    object_list<double*> doubleStarList = object_list<double*>({ &d1, &d2, &d3 });
    object_list<C> cList = object_list<C>({ C(3), C(5) });
    
    std::cout
        << intList << std::endl
        << floatList << std::endl
        << doubleStarList << std::endl
        << cList << std::endl;

    return 0;
}
