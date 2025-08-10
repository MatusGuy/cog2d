// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/math/vector.hpp"

namespace cog2d {

class Viewport;

class Camera
{
	friend class Viewport;

public:
	Viewport* m_viewport;
	Vector m_pos;
	Vector m_oldpos;
	Vector m_delta;

public:
	Camera();

	void pre_update();
	virtual void update() {}
	void post_update();
};

}  //namespace cog2d
