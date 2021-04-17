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
    int n1 = 0, n2 = 1, n3 = 2;

    object_list<int> listInt = object_list<int>();
    std::cout << listInt << std::endl;

    listInt = object_list<int>(std::allocator<int>());
    std::cout << listInt << std::endl;

    listInt = object_list<int>(3, 42);
    std::cout << listInt << std::endl;

    listInt = object_list<int>({ 0, 1, 2 }, std::allocator<int>());
    std::cout << listInt << std::endl;

    listInt = object_list<int>(listInt.begin(), listInt.end());
    std::cout << listInt << std::endl;

    listInt = object_list<int>((object_list<int>&&)listInt);
    std::cout << listInt << std::endl;

    listInt = object_list<int>((const object_list<int>&)listInt);
    std::cout << listInt << std::endl;

    listInt = object_list<int>((std::vector<int>)listInt);
    std::cout << listInt << std::endl;

    listInt = object_list<int>((const std::vector<int>&)listInt);
    std::cout << listInt << std::endl;

    return 0;
}
