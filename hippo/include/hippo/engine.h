#pragma once

#include "hippo/core/window.h"
#include "hippo/managers/logmanager.h"
#include "hippo/managers/rendermanager.h"

namespace hippo
{
    class Engine
    {
    public:
        static Engine& Instance();
        ~Engine() {}

        void Run();
        inline void Quit() { mIsRunning = false; }

        inline core::Window& GetWindow() { return mWindow; }

        // Managers
        inline managers::RenderManager& GetRenderManager() { return mRenderManager; }

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
        managers::RenderManager mRenderManager;

        // Singleton
        Engine();
        static Engine* mInstance;
    };
}