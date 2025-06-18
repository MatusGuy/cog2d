// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/math/vector.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Viewport;

class Camera
{
	friend class Viewport;

public:
	Viewport* m_viewport;
	Vector m_pos;

public:
	Camera();

	virtual void update() {}
};

COG2D_NAMESPACE_END_DECL
