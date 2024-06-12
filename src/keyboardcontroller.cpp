#include "keyboardcontroller.hpp"

KeyboardController::KeyboardController()
{

}

void KeyboardController::event(SDL_Event* ev)
{

	switch (ev->type)
	{
	case SDL_KEYDOWN:
	{
		SDL_KeyboardEvent kev = ev->key;

		//m_actions[];
		break;
	}
	}
}
