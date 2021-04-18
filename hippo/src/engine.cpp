#include "engine.h"
#include <iostream>

#include "sdl2/SDL.h"

namespace hippo
{
    void GetInfo()
    {
#ifdef HIPPO_CONFIG_DEBUG
        std::cout << "Configuration: DEBUG" << std::endl;
#endif
#ifdef HIPPO_CONFIG_RELEASE
        std::cout << "Configuration: RELEASE" << std::endl;
#endif
#ifdef HIPPO_PLATFORM_WINDOWS
        std::cout << "Platform: WINDOWS" << std::endl;
#endif
#ifdef HIPPO_PLATFORM_LINUX
        std::cout << "Platform: LINUX" << std::endl;
#endif
#ifdef HIPPO_PLATFORM_MAC
        std::cout << "Platform: MAC" << std::endl;
#endif
    }

    bool Initialize()
    {
        bool ret = true;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "Error initializing SDL2: " << SDL_GetError() << std::endl;
            ret = false;
        }
        else
        {
            SDL_version version;
            SDL_VERSION(&version);
            std::cout << "SDL " << (int32_t)version.major << "." << (int32_t)version.minor << "." << (int32_t)version.patch << std::endl;
        }

        return ret;
    }

    void Shutdown()
    {
        SDL_Quit();
    }
}