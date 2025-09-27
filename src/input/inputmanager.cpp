// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "inputmanager.hpp"

#include "cog2d/input/keyboardcontroller.hpp"
#include "cog2d/input/joypadcontroller.hpp"

namespace cog2d::input {

static struct
{
	std::vector<InputAction> actions;
	std::vector<Controller*> controllers;
} s_manager;

InputAction* register_action(InputAction& action)
{
	s_manager.actions.push_back(action);
	return &action;
}

void init(ProgramSettings& settings)
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

void add_controller(Controller* controller)
{
	s_manager.controllers.push_back(controller);

	for (auto& action : s_manager.actions) {
		controller->apply_action(&action);
	}

	controller->apply_finish();
}

void event(SDL_Event* ev)
{
	switch (ev->type) {
	case SDL_JOYDEVICEADDED: {
		SDL_JoyDeviceEvent jev = ev->jdevice;

		for (int i = 0; i < s_manager.controllers.size(); i++) {
			Controller* controller = s_manager.controllers[i];
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

		for (int i = 0; i < s_manager.controllers.size(); i++) {
			Controller* controller = s_manager.controllers[i];
			if (controller->get_type() == JoypadController::type() &&
			    static_cast<JoypadController*>(controller)->m_device_id == jev.which) {
				s_manager.controllers.erase(s_manager.controllers.begin() + i);
			}
		}
		return;
	}
	}

	for (Controller* controller : s_manager.controllers) {
		controller->event(ev);
	}
}

Controller* get_controller(uint8_t id)
{
	return static_cast<size_t>(id + 1) > get_controller_count() ? nullptr
	                                                            : s_manager.controllers[id];
}

std::size_t get_controller_count()
{
	return s_manager.controllers.size();
}

}  //namespace cog2d::input
