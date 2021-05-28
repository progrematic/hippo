#include "hippo/engine.h"
#include "hippo/log.h"

#include "hippo/graphics/mesh.h"
#include "hippo/graphics/shader.h"

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

	void Engine::Run()
	{
		if (Initialize())
		{
			// Test Mesh
			float vertices[]
			{
				 0.5f,  0.5f, 0.f,
				 0.5f, -0.5f, 0.f,
				-0.5f, -0.5f, 0.f,
				-0.5f,  0.5f, 0.f
			};
			uint32_t elements[]
			{
				0, 3, 1,
				1, 3, 2
			};
			std::shared_ptr<graphics::Mesh> mesh = std::make_shared<graphics::Mesh>(&vertices[0], 4, 3, &elements[0], 6);

			// Test Shader
			const char* vertexShader = R"(
				#version 410 core
				layout (location = 0) in vec3 position;
				out vec3 vpos;
				void main()
				{
					vpos = position + vec3(0.5, 0.5, 0);
					gl_Position = vec4(position, 1.0);
				}
			)";

			const char* fragmentShader = R"(
				#version 410 core
				out vec4 outColor;
				in vec3 vpos;

				uniform vec3 color = vec3(0.0);
				void main()
				{
					outColor = vec4(vpos, 1.0);
				}
			)";
			std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vertexShader, fragmentShader);
			shader->SetUniformFloat3("color", 1, 0, 0);

			//mRenderManager.SetWireframeMode(true);
			// core loop
			while (mIsRunning)
			{
				mWindow.PumpEvents();

				mWindow.BeginRender();

				auto rc = std::make_unique<graphics::rendercommands::RenderMesh>(mesh, shader);
				mRenderManager.Submit(std::move(rc));
				mRenderManager.Flush();

				mWindow.EndRender();
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
			mLogManager.Initialize();
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

		// Shutdown Managers - usually in reverse order
		mRenderManager.Shutdown();
		mLogManager.Shutdown();

		// Shutdown SDL
		mWindow.Shutdown();
		SDL_Quit();
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
	{}
}