#include "hippo/input/joystick.h"
#include "hippo/log.h"

#include <algorithm>

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_gamecontroller.h"

namespace hippo::input
{
	std::unordered_map<int, std::unique_ptr<Joystick::Controller>> Joystick::availableJoysticks;
	float Joystick::deadzone = 0.2f;

	void Joystick::OnJoystickConnected(SDL_ControllerDeviceEvent& e)
	{
		int deviceIndex = e.which;
		if (SDL_IsGameController(deviceIndex))
		{
			auto c = std::make_unique<Controller>();
			c->gc = SDL_GameControllerOpen(deviceIndex);
			if (c->gc)
			{
				c->joystickIndex = deviceIndex;
				std::fill(c->buttons.begin(), c->buttons.end(), false);
				std::fill(c->lastButtons.begin(), c->lastButtons.end(), false);
				std::fill(c->axes.begin(), c->axes.end(), 0.f);
				std::fill(c->lastAxes.begin(), c->lastAxes.end(), 0.f);

				int mapIndex = GetNextFreeIndex();
				HIPPO_TRACE("Joystick connected: mapIndex({}), deviceIndex({})", mapIndex, deviceIndex);
				availableJoysticks[mapIndex] = std::move(c);
			}
			else
			{
				HIPPO_ERROR("Error opening game controller with Device Index {}: {}", deviceIndex, SDL_GetError());
			}
		}
	}

	void Joystick::OnJoystickDisconnected(SDL_ControllerDeviceEvent& e)
	{
		int deviceIndex = e.which;
		for (auto it = availableJoysticks.begin(); it != availableJoysticks.end(); it++)
		{
			Controller* c = it->second.get();
			if (c->joystickIndex == deviceIndex)
			{
				HIPPO_TRACE("Joystick disconnected: {}", deviceIndex);
				SDL_GameControllerClose(c->gc);
				availableJoysticks.erase(it);
				break;
			}
		}
	}

	void Joystick::Shutdown()
	{
		for (auto it = availableJoysticks.begin(); it != availableJoysticks.end();)
		{
			Controller* c = it->second.get();
			SDL_GameControllerClose(c->gc);
			it = availableJoysticks.erase(it);
		}
	}

	void Joystick::Update()
	{
		for (auto it = availableJoysticks.begin(); it != availableJoysticks.end(); it++)
		{
			Controller* c = it->second.get();
			HIPPO_ASSERT(c && c->gc, "Invalid game controller in availableJoysticks map");
			if (c && c->gc)
			{
				c->lastButtons = c->buttons;
				c->lastAxes = c->axes;

				for (unsigned int i = 0; i < static_cast<int>(Button::Count); i++)
				{
					c->buttons[i] = SDL_GameControllerGetButton(c->gc, static_cast<SDL_GameControllerButton>(i));
				}

				for (unsigned int i = 0; i < static_cast<int>(Axis::Count); i++)
				{
					// SDL ranges axes from -32768 to 32767
					c->axes[i] = std::clamp(SDL_GameControllerGetAxis(c->gc, static_cast<SDL_GameControllerAxis>(i)) / 32767.f, -1.f, 1.f);
				}
			}
		}
	}

	bool Joystick::GetButton(int joystickId, Button button)
	{
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end())
		{
			return it->second->buttons[static_cast<int>(button)];
		}

		HIPPO_ERROR("Joystick with id {} is not available!", joystickId);
		return false;
	}

	bool Joystick::GetButtonDown(int joystickId, Button button)
	{
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end())
		{
			return it->second->buttons[static_cast<int>(button)] && !it->second->lastButtons[static_cast<int>(button)];
		}

		HIPPO_ERROR("Joystick with id {} is not available!", joystickId);
		return false;
	}

	bool Joystick::GetButtonUp(int joystickId, Button button)
	{
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end())
		{
			return !it->second->buttons[static_cast<int>(button)] && it->second->lastButtons[static_cast<int>(button)];
		}

		HIPPO_ERROR("Joystick with id {} is not available!", joystickId);
		return false;
	}

	float Joystick::GetAxis(int joystickId, Axis axis)
	{
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end())
		{
			float val = it->second->axes[static_cast<int>(axis)];
			return abs(val) > deadzone ? val : 0.f;
		}

		HIPPO_ERROR("Joystick with id {} is not available!", joystickId);
		return 0.f;
	}

	int Joystick::GetNextFreeIndex()
	{
		int ret = -1;

		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (availableJoysticks.count(i) == 0)
			{
				ret = i;
				break;
			}
		}

		return ret;
	}

	bool Joystick::IsJoystickAvailable(int joystickId)
	{
		return availableJoysticks.count(joystickId) > 0;
	}

}