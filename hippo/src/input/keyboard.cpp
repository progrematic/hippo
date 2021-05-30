#include "hippo/input/keyboard.h"
#include "hippo/log.h"

#include <algorithm>

#include "SDL2/SDL_keyboard.h"

namespace hippo::input
{
	std::array<bool, Keyboard::KeyCount> Keyboard::keys;
	std::array<bool, Keyboard::KeyCount> Keyboard::keysLast;

	void Keyboard::Initialize()
	{
		std::fill(keys.begin(), keys.end(), false);
		std::fill(keysLast.begin(), keysLast.end(), false);
	}

	void Keyboard::Update()
	{
		keysLast = keys;
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		for (int i = HIPPO_INPUT_KEY_FIRST; i < KeyCount; i++)
		{
			keys[i] = state[i];
		}
	}

	bool Keyboard::Key(int key)
	{
		HIPPO_ASSERT(key >= HIPPO_INPUT_KEY_FIRST && key < KeyCount, "Invalid keyboard key!");
		if (key >= HIPPO_INPUT_KEY_FIRST && key < KeyCount)
		{
			return keys[key];
		}
		return false;
	}

	bool Keyboard::KeyDown(int key)
	{
		HIPPO_ASSERT(key >= HIPPO_INPUT_KEY_FIRST && key < KeyCount, "Invalid keyboard key!");
		if (key >= HIPPO_INPUT_KEY_FIRST && key < KeyCount)
		{
			return keys[key] && !keysLast[key];
		}
		return false;
	}

	bool Keyboard::KeyUp(int key)
	{
		HIPPO_ASSERT(key >= HIPPO_INPUT_KEY_FIRST && key < KeyCount, "Invalid keyboard key!");
		if (key >= HIPPO_INPUT_KEY_FIRST && key < KeyCount)
		{
			return !keys[key] && keysLast[key];
		}
		return false;
	}
}