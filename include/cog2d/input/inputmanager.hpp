// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>
#include <map>
#include <functional>

#include <SDL2/SDL.h>

#include "cog2d/util/currenton.hpp"
#include "cog2d/input/controller.hpp"

namespace cog2d {
struct ProgramSettings;

struct InputAction
{
	uint8_t id;
	std::string name;
	std::string display_name;

	struct Config
	{
		uint8_t controller_type;

		/*!
		 * \brief input_id
		 *
		 * An input id can be a scancode if using
		 * a keyboard or a button id if using a
		 * joypad.
		 */
		int input_id;
	};

	std::vector<Config> configs;
};

namespace input {

InputAction* register_action(InputAction& action);

void init(ProgramSettings& settings);

void add_controller(Controller* controller);
std::size_t get_controller_count();
Controller* get_controller(uint8_t id);

void event(SDL_Event* ev);
}  //namespace input
}  //namespace cog2d
