// easylist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include "easylist.h"

//using namespace easylist;

std::string tostr(int x) { return std::to_string(x); }

class C
{
public:
    int n;
    C(int n) { this->n = n; }
    int get() { return n; }
    operator const std::string() const { return "Hello world!"; }
};

int main()
{
    static_assert(can_convert_string_v<std::string>);
    static_assert(can_convert_string_v<int>);
    static_assert(can_convert_string_v<float>);
    static_assert(can_convert_string_v<char>);
    static_assert(can_convert_string_v<bool>);
    static_assert(can_convert_string_v<char*>);
    static_assert(can_convert_string_v<double*>);
    static_assert(can_convert_string_v<long double[5]>);
    static_assert(can_convert_string_v<C>);

    double d = 3.14;
    long double lds[] = { 1.0, 1.0, 2.0, 3.0, 5.0 };
    C c = C(3);

    std::cout << convert_string<std::string>(std::string("Hello world!")) << std::endl;
    std::cout << convert_string<int>(2) << std::endl;
    std::cout << convert_string<float>(2.1f) << std::endl;
    std::cout << convert_string<char>('a') << std::endl;
    std::cout << convert_string<bool>(true) << std::endl;
    std::cout << convert_string<char*>((char*)"Hello world!") << std::endl;
    std::cout << convert_string<double*>(&d) << std::endl;
    std::cout << convert_string<long double[]>(lds) << std::endl;
    std::cout << convert_string<C>(c) << std::endl;
    
    return 0;
}
