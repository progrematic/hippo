#pragma once

#include "hippo/core/imguiwindow.h"

#include <string>
#include <memory>

#include "external/glm/glm.hpp"

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
		glm::vec3 clearColour;
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

		glm::ivec2 GetSize();

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