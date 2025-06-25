// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL.h>

#include "cog2d/input/controller.hpp"

namespace cog2d {

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

}
