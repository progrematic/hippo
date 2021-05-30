#include "hippo/input/mouse.h"
#include "hippo/log.h"

#include <algorithm>

#include "SDL2/SDL_mouse.h"

namespace hippo::input
{
	int Mouse::x = 0;
	int Mouse::xLast = 0;
	int Mouse::y = 0;
	int Mouse::yLast = 0;

	std::array<bool, Mouse::ButtonCount> Mouse::buttons;
	std::array<bool, Mouse::ButtonCount> Mouse::buttonsLast;

	void Mouse::Initialize()
	{
		std::fill(buttons.begin(), buttons.end(), false);
		std::fill(buttonsLast.begin(), buttonsLast.end(), false);
	}

	void Mouse::Update()
	{
		xLast = x;
		yLast = y;
		buttonsLast = buttons;	// TODO Optimization: is memcpy faster?
		Uint32 state = SDL_GetMouseState(&x, &y);
		for (int i = 0; i < ButtonCount; i++)
		{
			buttons[i] = state & SDL_BUTTON(i + 1);
		}
	}

	bool Mouse::Button(int button)
	{
		HIPPO_ASSERT(button >= HIPPO_INPUT_MOUSE_FIRST && button <= HIPPO_INPUT_MOUSE_LAST, "Invalid mouse button!");
		if (button >= HIPPO_INPUT_MOUSE_FIRST && button <= HIPPO_INPUT_MOUSE_LAST)
		{
			return buttons[button - 1];
		}
		return false;
	}

	bool Mouse::ButtonDown(int button)
	{
		HIPPO_ASSERT(button >= HIPPO_INPUT_MOUSE_FIRST && button <= HIPPO_INPUT_MOUSE_LAST, "Invalid mouse button!");
		if (button >= HIPPO_INPUT_MOUSE_FIRST && button <= HIPPO_INPUT_MOUSE_LAST)
		{
			return buttons[button - 1] && !buttonsLast[button - 1];
		}
		return false;
	}

	bool Mouse::ButtonUp(int button)
	{
		HIPPO_ASSERT(button >= HIPPO_INPUT_MOUSE_FIRST && button <= HIPPO_INPUT_MOUSE_LAST, "Invalid mouse button!");
		if (button >= HIPPO_INPUT_MOUSE_FIRST && button <= HIPPO_INPUT_MOUSE_LAST)
		{
			return !buttons[button - 1] && buttonsLast[button - 1];
		}
		return false;
	}
}