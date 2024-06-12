#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <map>

#include <SDL2/SDL.h>

struct InputAction;

class Controller
{
public:
	Controller();

	void apply_control(InputAction* action);
	void apply_finish();

	virtual void event(SDL_Event* ev) = 0;

	bool held(uint8_t id);

private:
	std::map<int, uint8_t> m_actions;
	std::map<uint8_t, bool> m_held;
};

#endif // CONTROLLER_HPP
