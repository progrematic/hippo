#pragma once

#include "hippo/core/imguiwindow.h"

#include <string>

struct SDL_Window;
using SDL_GLContext = void*;
namespace hippo::core
{
	struct WindowProperties
	{
		std::string title;
		int x, y, w, h;
		int wMin, hMin;
		int flags;
		float ccR, ccG, ccB;
		ImguiWindowProperties imguiProps;

		WindowProperties();
	};

	class Window
	{
	public:
		Window();
		~Window();

		bool Create(const WindowProperties& props);
		void Shutdown();

		void PumpEvents();

		void GetSize(int& w, int& h);

		SDL_Window* GetSDLWindow() { return mWindow; }
		SDL_GLContext GetGLContext() { return mGLContext; }

		void BeginRender();
		void EndRender();

	private:
		SDL_Window* mWindow;
		SDL_GLContext mGLContext;
		ImguiWindow mImguiWindow;
	};
}