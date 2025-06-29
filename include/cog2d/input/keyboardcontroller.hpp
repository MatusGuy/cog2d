// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/input/controller.hpp"

namespace cog2d {

class KeyboardController : public Controller
{
	COG2D_CONTROLLER_TYPE(1)

public:
	KeyboardController();

	void event(SDL_Event* ev) override;
};

}
