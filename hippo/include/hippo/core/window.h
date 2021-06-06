#pragma once

#include "hippo/core/imguiwindow.h"

#include <string>
#include <memory>

struct SDL_Window;
using SDL_GLContext = void*;

namespace hippo::graphics
{
	class Framebuffer;
}

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

		inline SDL_Window* GetSDLWindow() { return mWindow; }
		inline SDL_GLContext GetGLContext() { return mGLContext; }
		inline graphics::Framebuffer* GetFramebuffer() { return mFramebuffer.get(); }

		void BeginRender();
		void EndRender();

	private:
		SDL_Window* mWindow;
		SDL_GLContext mGLContext;
		ImguiWindow mImguiWindow;
		std::shared_ptr<graphics::Framebuffer> mFramebuffer;
	};
}