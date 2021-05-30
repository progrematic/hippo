#pragma once

#include <array>

namespace hippo::input
{
	class Mouse
	{
	public:
		static void Initialize();
		static void Update();

		inline static int X() { return x; }
		inline static int Y() { return y; }

		inline static int DX() { return x - xLast; }
		inline static int DY() { return y - yLast; }

		static bool Button(int button);
		static bool ButtonDown(int button);
		static bool ButtonUp(int button);

	private:
		constexpr static const int ButtonCount = 5;	// SDL supports up to 5 mouse buttons - see SDL_BUTTON_LEFT

		static int x, xLast;
		static int y, yLast;

		static std::array<bool, ButtonCount> buttons;
		static std::array<bool, ButtonCount> buttonsLast;
	};
}

// Adapted from SDL - see SDL_BUTTON_LEFT
enum
{
	HIPPO_INPUT_MOUSE_FIRST = 1,
	HIPPO_INPUT_MOUSE_LEFT = HIPPO_INPUT_MOUSE_FIRST,
	HIPPO_INPUT_MOUSE_MIDDLE = 2,
	HIPPO_INPUT_MOUSE_RIGHT = 3,
	HIPPO_INPUT_MOUSE_X1 = 4,
	HIPPO_INPUT_MOUSE_X2 = 5,
	HIPPO_INPUT_MOUSE_LAST = 5
};
