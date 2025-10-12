// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>
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

enum JoyHat : int
{
	HAT_UP = -1,
	HAT_DOWN = -2,
	HAT_LEFT = -3,
	HAT_RIGHT = -4,

	HAT_COUNT = 4
};

struct JoyAxisConverter
{
	ActionId negative = 0;
	ActionId positive = 0;

	inline bool is_valid() { return positive != negative; }
};

struct InputAction
{
	ActionId id;
	std::string display_name;

	SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;
	int joybutton;
	SDL_GameControllerButton gamectrlbutton;

	inline bool is_valid() { return scancode != SDL_SCANCODE_UNKNOWN; }
};

namespace input {

void register_action(InputAction&& action);
void register_joy_axis_converter(std::uint8_t axis, JoyAxisConverter converter);

void init(ProgramSettings& settings);
void deinit();

std::size_t controller_count();

void event(SDL_Event* ev);

bool hold(ControllerId player, ActionId action);
}  //namespace input
}  //namespace cog2d
