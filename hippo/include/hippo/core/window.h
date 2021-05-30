#pragma once

struct SDL_Window;
using SDL_GLContext = void*;
namespace hippo::core
{
	class Window
	{
	public:
		Window();
		~Window();

		bool Create();
		void Shutdown();

		void PumpEvents();

		void GetSize(int& w, int& h);

		void BeginRender();
		void EndRender();

	private:
		SDL_Window* mWindow;
		SDL_GLContext mGLContext;
	};
}