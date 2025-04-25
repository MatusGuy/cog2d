#ifndef JOYPADCONTROLLER_H
#define JOYPADCONTROLLER_H

#include <SDL2/SDL.h>

#include <controller.hpp>

class JoypadController : public Controller
{
	COG2D_CONTROLLER_TYPE(2)

public:
	SDL_JoystickID m_device_id;

public:
	JoypadController(SDL_JoystickID id);
	~JoypadController();

	virtual void event(SDL_Event* ev) override;
};

#endif  // JOYPADCONTROLLER_H
