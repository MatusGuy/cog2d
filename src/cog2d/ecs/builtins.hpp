#pragma once

#include <cstdint>

#include "cog2d/ecs/entitybase.hpp"
#include "cog2d/util/timing.hpp"

/**
 * Built in components and systems in cog2d's ECS
 */

#include "cog2d/ecs/builtins/texture.hpp"
#include "cog2d/ecs/builtins/properties.hpp"
#include "cog2d/ecs/builtins/collision/collisionsystem.hpp"

namespace cog2d {

enum Component
{
	COMP_COLLISION = 1 << 0,
	COMP_GRAVITY = 1 << 2,
	COMP_TEXTURE = 1 << 3,
	COMP_SPRITE = 1 << 4,
	COMP_PROPERTIES = 1 << 5,

	COMP_COUNT = 2
};

struct CompCollision;

namespace systems {
void velocity_update(EntityBase& ent, CompCollision& col);
}  //namespace systems

union CompGraphic
{
	CompTexture texture;
	//CompSprite sprite;
};

struct CompGravity
{
	Vector accel;

	/// gravitational acceleration
	float grav = 1;
};
}  //namespace cog2d
