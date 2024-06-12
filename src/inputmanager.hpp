#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <string>
#include <map>
#include <functional>

#include <SDL2/SDL.h>

#include "types.hpp"
#include "controller.hpp"

struct InputAction {
	std::string name;
	std::string display_name;

	uint8_t id;

	/**
	 * @brief input_id
	 *
	 * An input id can be a scancode if using
	 * a keyboard or a button id if using a
	 * joypad.
	 */
	int input_id;
};

class InputManager : public Singleton<InputManager>
{
public:
	uint8_t m_max_concurrent_players = 255;

public:
	InputManager();

	void event(SDL_Event* ev);

private:
	std::map<uint8_t, Controller*> m_controllers;
};

#endif // INPUTMANAGER_HPP
