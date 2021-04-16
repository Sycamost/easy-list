// easylist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "easylist.h"

class Class
{};

using namespace easylist;

int main()
{
    object_list<int> t = object_list<int>(42);
    t.displayData();
    std::cout << t.getData() << std::endl;
    return 0;
}
