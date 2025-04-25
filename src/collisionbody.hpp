#ifndef COLLISIONBODY_HPP
#define COLLISIONBODY_HPP

#include <vector>

#include "collisionsystem.hpp"
#include "types.hpp"

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

#endif  // COLLISIONBODY_HPP
