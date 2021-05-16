#include "core/window.h"
#include "engine.h"
#include "log.h"

#include "SDL2/SDL.h"
#include "glad/glad.h"

namespace hippo::core
{
	Window::Window() : mWindow(nullptr) {}
	Window::~Window()
	{
		if (mWindow)
		{
			Shutdown();
		}
	}

	bool Window::Create()
	{
		mWindow = SDL_CreateWindow("HippoGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!mWindow)
		{
			HIPPO_ERROR("Error creating window: {}", SDL_GetError());
			return false;
		}

#ifdef HIPPO_PLATFORM_MAC
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_SetWindowMinimumSize(mWindow, 200, 200);

		mGLContext = SDL_GL_CreateContext(mWindow);
		if (mGLContext == nullptr)
		{
			HIPPO_ERROR("Error creating OpenGL context: {}", SDL_GetError());
			return false;
		}

		gladLoadGLLoader(SDL_GL_GetProcAddress);

		// TODO: Move this to a renderer initialization
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(
			static_cast<float>(0x64) / static_cast<float>(0xFF),
			static_cast<float>(0x95) / static_cast<float>(0xFF),
			static_cast<float>(0xED) / static_cast<float>(0xFF),
			1
		);	// cornflower blue

		return true;
	}

	void Window::Shutdown()
	{
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
	}

	void Window::PumpEvents()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				Engine::Instance().Quit();
				break;

			default:
				break;
			}
		}
	}

	void Window::BeginRender()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::EndRender()
	{
		SDL_GL_SwapWindow(mWindow);
	}
}