#include "hippo/engine.h"
#include "hippo/log.h"
#include "hippo/app.h"

#include "hippo/input/mouse.h"
#include "hippo/input/keyboard.h"

#include "SDL2/SDL.h"

namespace hippo
{
	Engine& Engine::Instance()
	{
		if (!mInstance)
		{
			mInstance = new Engine();
		}

		return *mInstance;
	}

	void Engine::Run(App* app)
	{
		mLogManager.Initialize();
		HIPPO_ASSERT(!mApp, "Attempting to call Engine::Run when a valid App already exists");
		if (mApp)
		{
			return;
		}

		mApp = app;
		if (Initialize())
		{
			// core loop
			while (mIsRunning)
			{
				Update();
				Render();
			}

			Shutdown();
		}
	}



	// private

	bool Engine::Initialize()
	{
		bool ret = false;

		HIPPO_ASSERT(!mIsInitialized, "Attempting to call Engine::Initialize() more than once!");
		if (!mIsInitialized)
		{
			GetInfo();

			if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			{
				HIPPO_ERROR("Error initializing SDL2: {}", SDL_GetError());
			}
			else
			{
				SDL_version version;
				SDL_VERSION(&version);
				HIPPO_INFO("SDL {}.{}.{}", (int32_t)version.major, (int32_t)version.minor, (int32_t)version.patch);

				if (mWindow.Create())
				{
					// Initialize Managers
					mRenderManager.Initialize();

					ret = true;
					mIsRunning = true;
					mIsInitialized = true;

					// Initialize input
					input::Mouse::Initialize();
					input::Keyboard::Initialize();

					// Initialize Client
					mApp->Initialize();
				}
			}

			if (!ret)
			{
				HIPPO_ERROR("Engine initialization failed. Shutting down.");
				Shutdown();
			}
		}

		return ret;
	}

	void Engine::Shutdown()
	{
		mIsRunning = false;
		mIsInitialized = false;

		// Shutdown client
		mApp->Shutdown();

		// Shutdown Managers - usually in reverse order
		mRenderManager.Shutdown();

		// Shutdown SDL
		mWindow.Shutdown();
		SDL_Quit();

		mLogManager.Shutdown();
	}

	void Engine::Update()
	{
		mWindow.PumpEvents();
		mApp->Update();
	}

	void Engine::Render()
	{
		mWindow.BeginRender();
		mApp->Render();
		mWindow.EndRender();
	}

    void Engine::GetInfo()
    {
		HIPPO_TRACE("HippoEngine v{}.{}", 0, 1);
#ifdef HIPPO_CONFIG_DEBUG
		HIPPO_DEBUG("Configuration: DEBUG");
#endif
#ifdef HIPPO_CONFIG_RELEASE
		HIPPO_DEBUG("Configuration: RELEASE");
#endif
#ifdef HIPPO_PLATFORM_WINDOWS
		HIPPO_WARN("Platform: WINDOWS");
#endif
#ifdef HIPPO_PLATFORM_LINUX
		HIPPO_WARN("Platform: LINUX");
#endif
#ifdef HIPPO_PLATFORM_MAC
		HIPPO_WARN("Platform: MAC");
#endif
    }

	// singleton
	Engine* Engine::mInstance = nullptr;

	Engine::Engine() 
		: mIsRunning(false)
		, mIsInitialized(false)
		, mApp(nullptr)
	{}
}