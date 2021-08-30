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
	class VertexArray;
	class Shader;
}

namespace hippo::core
{
	struct WindowProperties
	{
		std::string title;
		int x, y, w, h;
		int wMin, hMin;
		int flags;
		float aspectRatio;
		glm::vec3 clearColour;
		ImguiWindowProperties imguiProps;

		WindowProperties();
	};

	class Window
	{
	public:
		Window();
		~Window();

		inline void SetShouldRenderToScreen(bool render) { mShouldRenderToScreen = render; }

		bool Create(const WindowProperties& props);
		void Shutdown();

		void PumpEvents();

		glm::ivec2 GetSize();

		inline SDL_Window* GetSDLWindow() { return mWindow; }
		inline SDL_GLContext GetGLContext() { return mGLContext; }
		inline graphics::Framebuffer* GetFramebuffer() { return mFramebuffer.get(); }

		void BeginRender();
		void EndRender();

		glm::ivec2 GetSizeInAspectRatio(int width, int height);

	private:
		void InitializeScreenRender();
		void RenderToScreen();
		void HandleResize(int width, int height);

	private:
		WindowProperties mWindowProperties;
		SDL_Window* mWindow;
		SDL_GLContext mGLContext;
		ImguiWindow mImguiWindow;
		std::shared_ptr<graphics::Framebuffer> mFramebuffer;

		// Screen Render
		bool mShouldRenderToScreen;
		glm::vec2 mFramebufferSize;
		std::shared_ptr<graphics::VertexArray> mScreenVA;
		std::shared_ptr<graphics::Shader> mScreenShader;
	};
}