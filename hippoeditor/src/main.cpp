#include <iostream>
#include "hippo/engine.h"

int main()
{
    std::cout << "Hello, Youtube!" << std::endl;
    hippo::GetInfo();

    hippo::Initialize();
    hippo::Shutdown();

#ifdef HIPPO_PLATFORM_WINDOWS
    system("pause");
#endif
    return 0;
}