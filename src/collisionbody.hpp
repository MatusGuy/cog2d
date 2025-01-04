#ifndef COLLISIONBODY_HPP
#define COLLISIONBODY_HPP

#include <vector>

#include "types.hpp"

class CollisionBody
{
public:
	Vector m_movement;
	Rect m_bbox;

	/**
	 * This int refers to a collision group in the
	 * CollisionSystem's collision group index.
	 */
	int m_group;

public:
	CollisionBody();

	Rect get_dest();
	void apply_movement();
};

#endif // COLLISIONBODY_HPP
