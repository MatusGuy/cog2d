#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <string>
#include <map>
#include <functional>

#include <SDL2/SDL.h>

#include "types.hpp"
#include "controller.hpp"

struct InputAction {
	uint8_t id;
	std::string name;
	std::string display_name;

	struct Config {
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

#define COG2D_USE_INPUTMANAGER COG2D_USING(InputManager, inputmanager)
class InputManager : public Singleton<InputManager>
{
public:
	uint8_t m_max_concurrent_players = 255;

public:
	InputManager();

	InputAction* register_action(InputAction& action);

	void init();

	void add_controller(Controller* controller);
	inline size_t get_controller_count() { return m_controllers.size(); }
	inline Controller* get_controller(uint8_t id) {
		return static_cast<size_t>(id+1) > get_controller_count() ? nullptr : m_controllers[id];
	}

	void event(SDL_Event* ev);

private:
	std::vector<InputAction> m_actions;
	std::vector<Controller*> m_controllers;
};

#endif // INPUTMANAGER_HPP
