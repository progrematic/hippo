#pragma once

#include "core/window.h"
#include "managers/logmanager.h"
#include "managers/rendermanager.h"

namespace hippo
{
    class Engine
    {
    public:
        static Engine& Instance();
        ~Engine() {}

        void Run();
        inline void Quit() { mIsRunning = false; }

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