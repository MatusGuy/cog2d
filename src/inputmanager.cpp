#include "inputmanager.hpp"

InputManager::InputManager()
{

}

void InputManager::event(SDL_Event* ev)
{
	for (auto& [key, controller] : m_controllers)
	{
		controller->event(ev);
	}
}
