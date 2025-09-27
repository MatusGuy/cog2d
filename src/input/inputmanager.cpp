// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "inputmanager.hpp"

#include <unordered_map>

#include "cog2d/input/keyboardcontroller.hpp"
#include "cog2d/input/joypadcontroller.hpp"

namespace cog2d::input {

/// Maximum amount of keys on an input device
#define COG2D_MAX_KEYS (SDL_NUM_SCANCODES)
#define COG2D_MAX_PLAYERS (4)
#define COG2D_NUM_ACTIONS (10)

static struct
{
	std::vector<InputAction> actions;
	bool held_menu[MENU_COUNT];
	bool held_actions[COG2D_MAX_PLAYERS][COG2D_NUM_ACTIONS];
	ActionId action_map[COG2D_MAX_PLAYERS][COG2D_MAX_KEYS];
} s_manager;

InputAction* register_action(InputAction& action)
{
	s_manager.actions.push_back(action);
	return &action;
}

void init(ProgramSettings& settings)
{
	for (int i = 0; i < s_manager.actions.size(); ++i) {
		std::uint32_t key = s_manager.actions[i].configs[0].input_id;
		s_manager.action_map[0][key] = i;
	}
}

void add_controller(Controller* controller)
{
	/*
	s_manager.controllers.push_back(controller);

	for (auto& action : s_manager.actions) {
		controller->apply_action(&action);
	}

	controller->apply_finish();
	*/
}

void event(SDL_Event* ev)
{
	switch (ev->type) {
	case SDL_JOYDEVICEADDED: {
		/*
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
		*/
		break;
	}

	case SDL_JOYDEVICEREMOVED: {
		/*
		SDL_JoyDeviceEvent jev = ev->jdevice;

		for (int i = 0; i < s_manager.controllers.size(); i++) {
			Controller* controller = s_manager.controllers[i];
			if (controller->get_type() == JoypadController::type() &&
				static_cast<JoypadController*>(controller)->m_device_id == jev.which) {
				s_manager.controllers.erase(s_manager.controllers.begin() + i);
			}
		}
		*/
		break;
	}

	case SDL_KEYDOWN: {
		ActionId action = s_manager.action_map[0][ev->key.keysym.scancode];
		s_manager.held_actions[0][action] = true;
		break;
	}

	case SDL_KEYUP: {
		ActionId action = s_manager.action_map[0][ev->key.keysym.scancode];
		s_manager.held_actions[0][action] = false;
		break;
	}
	}
}

/*
Controller* get_controller(uint8_t id)
{
	return static_cast<size_t>(id + 1) > get_controller_count() ? nullptr
																: s_manager.controllers[id];
}
*/

std::size_t controller_count()
{
	//return s_manager.controllers.size();
	return 1;
}

bool hold(ControllerId player, ActionId action)
{
	return s_manager.held_actions[player][action];
}

}  //namespace cog2d::input
