#pragma once

#include "core/window.h"
#include "managers/logmanager.h"

namespace hippo
{
    class Engine
    {
    public:
        static Engine& Instance();
        ~Engine() {}

        void Run();
        inline void Quit() { mIsRunning = false; }

	private:
		void GetInfo();
		[[nodiscard]] bool Initialize();
		void Shutdown();

    private:
        bool mIsRunning;
        bool mIsInitialized;
        core::Window mWindow;

        // Managers
        managers::LogManager mLogManager;

        // Singleton
        Engine();
        static Engine* mInstance;
    };
}