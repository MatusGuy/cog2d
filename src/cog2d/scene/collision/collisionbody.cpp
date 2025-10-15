// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "collisionbody.hpp"

namespace cog2d {

CollisionBody::CollisionBody()
    : m_bbox(0, 0, 0, 0),
      m_mov(0, 0)
{
}

Rect CollisionBody::get_dest()
{
	Rect rect = m_bbox;
	rect.pos += m_mov;
	return rect;
}

void CollisionBody::apply_movement()
{
	m_bbox = get_dest();
	m_mov.x = 0;
	m_mov.y = 0;
}

}
