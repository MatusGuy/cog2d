// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>

#include "cog2d/scene/collision/collisionsystem.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {

class CollisionBody
{
public:
	Vector m_mov;
	Rect m_bbox;

	// TODO: improve upon this - maybe make some sort of
	// priority system?
	bool m_static;

	/**
	 * This int refers to a collision group in the
	 * CollisionSystem's collision group index.
	 */
	int m_group;

public:
	CollisionBody();

	Rect get_dest();
	void apply_movement();

	virtual CollisionSystem::Response collision(CollisionBody* other)
	{
		return CollisionSystem::COLRESP_ACCEPT;
	}
};

}
