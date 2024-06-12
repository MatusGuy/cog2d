#include "keyboardcontroller.hpp"

#include <iostream>

KeyboardController::KeyboardController():
	Controller()
{

}

void KeyboardController::event(SDL_Event* ev)
{
	switch (ev->type)
	{

	case SDL_KEYDOWN:
	{
		SDL_KeyboardEvent kev = ev->key;

		auto actionid = m_actions.find(kev.keysym.scancode);
		if (actionid == m_actions.end())
			break;

		m_held[actionid->second] = true;

		break;
	}

	case SDL_KEYUP:
	{
		SDL_KeyboardEvent kev = ev->key;

		auto actionid = m_actions.find(kev.keysym.scancode);
		if (actionid == m_actions.end())
			break;

		m_held[actionid->second] = false;

		break;
	}

	default:
		break;

	}
}
