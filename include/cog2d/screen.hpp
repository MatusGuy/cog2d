// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL_events.h>

#include "cog2d/util/namespacedef.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Screen
{
public:
	virtual ~Screen() {}

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool event(SDL_Event* ev) = 0;
};

COG2D_NAMESPACE_END_DECL
