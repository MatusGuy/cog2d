#pragma once

#include <cstdint>

#include "cog2d/ecs/entitybase.hpp"
#include "cog2d/util/timing.hpp"

/**
 * Built in components and systems in cog2d's ECS
 */

#include "cog2d/ecs/builtins/texture.hpp"

namespace cog2d {

union CompGraphic
{
	CompTexture texture;
	//CompSprite sprite;
};

}  //namespace cog2d

// TODO: Move these to seperate files?
namespace cog2d {

enum Component
{
	COMP_COLLISION = 1 << 0,
	COMP_GRAVITY = 1 << 2,
	COMP_TEXTURE = 1 << 3,
	COMP_SPRITE = 1 << 4,

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
void velocity_update(EntityBase& ent, CompCollision& col);
}  //namespace systems

struct CompGravity
{
	Vector accel;

	/// gravitational acceleration
	float grav = 1;
};
}  //namespace cog2d
