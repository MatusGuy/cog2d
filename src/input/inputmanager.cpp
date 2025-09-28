// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "inputmanager.hpp"

namespace cog2d::input {

/// Maximum amount of keys on an input device
#define COG2D_MAX_KEYS (SDL_NUM_SCANCODES)
#define COG2D_MAX_PLAYERS (4)
#define COG2D_NUM_ACTIONS (10)

static struct
{
	InputAction actions[COG2D_NUM_ACTIONS];
	bool held_menu[MENU_COUNT];
	bool held_actions[COG2D_MAX_PLAYERS][COG2D_NUM_ACTIONS];
	ActionId action_map[COG2D_MAX_PLAYERS][COG2D_MAX_KEYS];
} s_manager;

void register_action(InputAction&& action)
{
	s_manager.actions[action.id] = action;
}

void init(ProgramSettings& settings)
{
	// right now, player mapping is hardcoded to first player
	// solution is to have a screen where the first player to hit a button on their controller
	// becomes the first player and so on and can change their inputs bla bla bla... See other
	// multiplayer games like Spelunky or Idk smash bros. When the game starts, control bindings are
	// correctly assigned to each player
	for (int i = 0; i < COG2D_NUM_ACTIONS; ++i) {
		s_manager.action_map[0][s_manager.actions[i].scancode] = i;
	}
}

void event(SDL_Event* ev)
{
	switch (ev->type) {
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

std::size_t controller_count()
{
	return 1;
}

bool hold(ControllerId player, ActionId action)
{
	return s_manager.held_actions[player][action];
}

}  //namespace cog2d::input
