#include "inputmanager.hpp"

#include "keyboardcontroller.hpp"

InputManager::InputManager():
	m_actions(),
	m_controllers()
{

}

InputAction* InputManager::register_action(InputAction action)
{
	m_actions.push_back(action);
}

void InputManager::init()
{
	// Keyboard is always connected... i think
	KeyboardController* controller = new KeyboardController;
	controller->m_id = 1;

	add_controller(controller);
}

void InputManager::add_controller(Controller* controller)
{
	m_controllers.push_back(controller);

	for (auto& action : m_actions)
	{
		controller->apply_action(&action);
	}

	controller->apply_finish();
}

void InputManager::event(SDL_Event* ev)
{
	for (auto& controller : m_controllers)
	{
		controller->event(ev);
	}
}

