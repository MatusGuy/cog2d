#pragma once

#include <cstdint>

#include "cog2d/ecs/entity.hpp"
#include "cog2d/util/timing.hpp"

/**
 * Built in components and system in cog2d's ECS
 */

namespace cog2d {

enum Component
{
	COMP_COLLISION = 1 << 0,
	COMP_GRAVITY = 1 << 2,

	COMP_COUNT = 2
};

struct CompCollision
{
	Vector mov;
	std::uint32_t group;

	/// static and immovable
	bool stabile;
};

namespace systems {
void velocity(EntityBase& ent, CompCollision& col)
{
	if (ent.comps & COMP_COLLISION) {
		col.mov = ent.vel;
		//if (ent.follow_camera)
		//	col.mov += viewport.get_camera()->m_delta;
	} else {
		Vector inc = ent.vel;
		//if (follow_camera())
		//	inc += viewport.get_camera()->m_delta;

		ent.bbox.pos += inc * velocity_multiplier();
	}
}
}  //namespace systems

struct CompGravity
{
	Vector accel;

	/// gravitational acceleration
	float grav = 1;
};
}  //namespace cog2d
