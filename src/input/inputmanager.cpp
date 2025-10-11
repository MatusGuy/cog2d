// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "inputmanager.hpp"

#include <cog2d/util/logger.hpp>

namespace cog2d::input {

/// Maximum amount of keys on an input device
#define COG2D_MAX_KEYS (SDL_NUM_SCANCODES)
#define COG2D_MAX_PLAYERS (4)
#define COG2D_MAX_JOYPADS (COG2D_MAX_PLAYERS * 2)
#define COG2D_NUM_ACTIONS (10)

static struct
{
	InputAction actions[COG2D_NUM_ACTIONS];
	bool held_menu[MENU_COUNT];
	bool held_actions[COG2D_MAX_PLAYERS][COG2D_NUM_ACTIONS];
	ActionId action_map[COG2D_MAX_PLAYERS][COG2D_MAX_KEYS];

	SDL_Joystick* joysticks[COG2D_MAX_JOYPADS] = {0};
	SDL_GameController* gamectrls[COG2D_MAX_JOYPADS] = {0};

	std::uint8_t kbd_player;
	std::uint8_t joy_player[COG2D_MAX_JOYPADS];
	std::uint8_t gamectrl_player[COG2D_MAX_JOYPADS];
} s_manager;

void register_action(InputAction&& action)
{
	s_manager.actions[action.id] = action;
}

ActionId& get_joy_action(ControllerId player, int button) {
	return s_manager.action_map[player][button + 4];
}

void init(ProgramSettings& settings)
{
	// have a screen where the first player to hit a button on their controller
	// becomes the first player and so on and can change their inputs bla bla bla... See other
	// multiplayer games like Spelunky or Idk smash bros. When the game starts, control bindings are
	// correctly assigned to each player

	//s_manager.kbd_player = 0;

	s_manager.joysticks[0] = SDL_JoystickOpen(0);
	s_manager.joy_player[0] = 0;

	//s_manager.gamectrls[0] = SDL_GameControllerOpen(1);
	//s_manager.gamectrl_player[0] = 0;

	for (int i = 0; i < COG2D_NUM_ACTIONS; ++i) {
		//s_manager.action_map[s_manager.kbd_player][s_manager.actions[i].scancode] = i;
		get_joy_action(s_manager.joy_player[0], s_manager.actions[i].joybutton) = i;
		//s_manager.action_map[s_manager.kbd_player][s_manager.actions[i].gamectrlbutton] = i;
	}
}

void deinit()
{
	SDL_Joystick* joy;
	for (int i = 0; i < COG2D_MAX_JOYPADS; ++i) {
		joy = s_manager.joysticks[i];
		if (joy != nullptr)
			SDL_JoystickClose(joy);
	}

	SDL_GameController* gamectrl;
	for (int i = 0; i < COG2D_MAX_JOYPADS; ++i) {
		gamectrl = s_manager.gamectrls[i];
		if (gamectrl != nullptr)
			SDL_GameControllerClose(gamectrl);
	}
}

void event(SDL_Event* ev)
{
	switch (ev->type) {
	case SDL_KEYDOWN: {
		ActionId action = s_manager.action_map[s_manager.kbd_player][ev->key.keysym.scancode];
		s_manager.held_actions[s_manager.kbd_player][action] = true;
		break;
	}

	case SDL_KEYUP: {
		ActionId action = s_manager.action_map[s_manager.kbd_player][ev->key.keysym.scancode];
		s_manager.held_actions[s_manager.kbd_player][action] = false;
		break;
	}

	case SDL_JOYBUTTONDOWN: {
		SDL_JoyButtonEvent& jev = ev->jbutton;
		ActionId action = get_joy_action(s_manager.joy_player[jev.which], jev.button);
		s_manager.held_actions[s_manager.joy_player[jev.which]][action] = true;
		break;
	}

	case SDL_JOYBUTTONUP: {
		SDL_JoyButtonEvent& jev = ev->jbutton;
		ActionId action = get_joy_action(s_manager.joy_player[jev.which], jev.button);
		s_manager.held_actions[s_manager.joy_player[jev.which]][action] = false;
		break;
	}

	case SDL_JOYHATMOTION: {
		SDL_JoyHatEvent& jev = ev->jhat;

		ActionId up    = get_joy_action(s_manager.joy_player[jev.which], HAT_UP);
		ActionId down  = get_joy_action(s_manager.joy_player[jev.which], HAT_DOWN);
		ActionId left  = get_joy_action(s_manager.joy_player[jev.which], HAT_LEFT);
		ActionId right = get_joy_action(s_manager.joy_player[jev.which], HAT_RIGHT);

		bool* actions = s_manager.held_actions[s_manager.joy_player[jev.which]];

		actions[up]    = jev.value & SDL_HAT_UP;
		actions[down]  = jev.value & SDL_HAT_DOWN;
		actions[left]  = jev.value & SDL_HAT_LEFT;
		actions[right] = jev.value & SDL_HAT_RIGHT;

		break;
	}

	case SDL_CONTROLLERBUTTONDOWN: {
		SDL_ControllerButtonEvent& cev = ev->cbutton;
		log::debug(fmt::format("{}", cev.button));
		ActionId action = s_manager.action_map[s_manager.gamectrl_player[cev.which]][cev.button];
		s_manager.held_actions[s_manager.gamectrl_player[cev.which]][action] = true;
		break;
	}

	case SDL_CONTROLLERBUTTONUP: {
		SDL_ControllerButtonEvent& cev = ev->cbutton;
		ActionId action = get_joy_action(s_manager.gamectrl_player[cev.which], cev.button);
		s_manager.held_actions[s_manager.gamectrl_player[cev.which]][action] = false;
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
