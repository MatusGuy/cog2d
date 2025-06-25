// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL_events.h>

namespace cog2d {

class Screen
{
public:
	virtual ~Screen() {}

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool event(SDL_Event* ev) = 0;
};

}
