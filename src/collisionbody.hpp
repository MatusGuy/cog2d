#ifndef COLLISIONBODY_HPP
#define COLLISIONBODY_HPP

#include "types.hpp"

class CollisionBody
{
public:
	Vector m_movement;
	Rect m_bbox;
	// TODO: Collision groups

	// HACK: Possibly a hack
	CollisionBody* m_last_collider;

public:
	CollisionBody();

	Rect get_destination();
	void apply_movement();
};

#endif // COLLISIONBODY_HPP
