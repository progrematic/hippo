#pragma once

#include "core/window.h"

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
        core::Window mWindow;

        // Singleton
        Engine();
        static Engine* mInstance;
    };
}