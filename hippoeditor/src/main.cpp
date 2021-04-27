#include <iostream>
#include "hippo/engine.h"

int main()
{
    hippo::Engine& engine = hippo::Engine::Instance();
    engine.Run();

    std::cout << "Press ENTER to continue...";
    std::cin.ignore();
    return 0;
}