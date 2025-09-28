// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>
#include <map>
#include <functional>
#include <cstdint>

#include <SDL2/SDL.h>

namespace cog2d {
struct ProgramSettings;

using ControllerId = std::uint8_t;
using ActionId = std::uint8_t;

enum MenuAction
{
	MENU_UP,
	MENU_DOWN,
	MENU_LEFT,
	MENU_RIGHT,
	MENU_ACCEPT,
	MENU_CANCEL,

	MENU_COUNT
};

struct InputAction
{
	ActionId id;
	std::string display_name;

	SDL_Scancode scancode;
	std::uint32_t joybutton;
	SDL_GameControllerButton gamectrlbutton;
};

namespace input {

void register_action(InputAction&& action);

void init(ProgramSettings& settings);

std::size_t controller_count();

void event(SDL_Event* ev);

bool hold(ControllerId player, ActionId action);
}  //namespace input
}  //namespace cog2d
