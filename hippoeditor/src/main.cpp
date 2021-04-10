#include <iostream>
#include "hippo/engine.h"

int main()
{
    std::cout << "Hello, Youtube!" << std::endl;
    hippo::GetInfo();
    int a = hippo::Add(10, 5);
    int b = hippo::Sub(10, 5);
    std::cout << a << ", " << b << std::endl;
    return 0;
}