#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <map>

#include <SDL2/SDL.h>

struct InputAction;

class Controller
{
public:
	uint8_t m_id = 0;

public:
	Controller();

	void apply_action(InputAction* action);
	void apply_finish();

	virtual void event(SDL_Event* ev) = 0;

	// 0 is reserved. These functions are meant to be overridden.
	static uint8_t type() { return 0; }
	virtual uint8_t get_type() = 0;

	bool held(uint8_t id) const;

protected:
	std::map<int, uint8_t> m_actions;
	std::map<uint8_t, bool> m_held;
};

#endif // CONTROLLER_HPP
