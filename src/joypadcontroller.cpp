#include "joypadcontroller.hpp"

#include <iostream>

JoypadController::JoypadController(SDL_JoystickID id):
	m_device_id(id)
{
	SDL_JoystickOpen(id);
}

JoypadController::~JoypadController()
{
	SDL_JoystickClose(SDL_JoystickFromInstanceID(m_device_id));
}

void JoypadController::event(SDL_Event* ev)
{
	switch (ev->type)
	{
	case SDL_JOYBUTTONDOWN: {
		SDL_JoyButtonEvent jev = ev->jbutton;

		auto actionid = m_actions.find(static_cast<int>(jev.button));
		if (actionid == m_actions.end())
			break;

		m_held[actionid->second] = true;
		break;
	}

	case SDL_JOYBUTTONUP: {
		SDL_JoyButtonEvent jev = ev->jbutton;

		auto actionid = m_actions.find(static_cast<int>(jev.button));
		if (actionid == m_actions.end())
			break;

		m_held[actionid->second] = false;
		break;
	}
	};
}
