// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "inputmanager.hpp"

#include "cog2d/input/keyboardcontroller.hpp"
#include "cog2d/input/joypadcontroller.hpp"

namespace cog2d {

InputManager::InputManager()
    : m_actions(),
      m_controllers()
{
}

InputAction* InputManager::register_action(InputAction& action)
{
	m_actions.push_back(action);
	return &action;
}

void InputManager::init()
{
	// The keyboard should always be there for you :)
	KeyboardController* keyboard = new KeyboardController;
	//keyboard->m_id = 1;

	add_controller(keyboard);

	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		JoypadController* jcontroller = new JoypadController(i);

		add_controller(jcontroller);
	}
}

void InputManager::add_controller(Controller* controller)
{
	m_controllers.push_back(controller);

	for (auto& action : m_actions) {
		controller->apply_action(&action);
	}

	controller->apply_finish();
}

void InputManager::event(SDL_Event* ev)
{
	switch (ev->type) {
	case SDL_JOYDEVICEADDED: {
		SDL_JoyDeviceEvent jev = ev->jdevice;

		for (int i = 0; i < m_controllers.size(); i++) {
			Controller* controller = m_controllers[i];
			if (controller->get_type() == JoypadController::type() &&
			    static_cast<JoypadController*>(controller)->m_device_id == jev.which) {
				// joypad already exists
				break;
			}
		}

		JoypadController* jcontroller = new JoypadController(jev.which);

		add_controller(jcontroller);
		return;
	}

	case SDL_JOYDEVICEREMOVED: {
		SDL_JoyDeviceEvent jev = ev->jdevice;

		for (int i = 0; i < m_controllers.size(); i++) {
			Controller* controller = m_controllers[i];
			if (controller->get_type() == JoypadController::type() &&
			    static_cast<JoypadController*>(controller)->m_device_id == jev.which) {
				m_controllers.erase(m_controllers.begin() + i);
			}
		}
		return;
	}
	}

	for (Controller* controller : m_controllers) {
		controller->event(ev);
	}
}

}  //namespace cog2d
