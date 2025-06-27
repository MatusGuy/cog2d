// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/math/rect.hpp"

#define COG2D_ACTOR_COMPONENT(t) constexpr static std::uint8_t type = t;

namespace cog2d {
namespace ActorComps {

struct Component {};

struct Geometry : public Component, public Rect
{
	COG2D_ACTOR_COMPONENT(0)
};

struct Velocity : public Component, public Vector
{
	COG2D_ACTOR_COMPONENT(1)
};

struct Gravity : public Component
{
	COG2D_ACTOR_COMPONENT(2)

	Vector accel;

	/// Gravity modifier
	float grav;
};

struct Collision : public Component
{
	COG2D_ACTOR_COMPONENT(3)

	Vector mov;
	int group;

	// TODO: improve upon this - maybe make some sort of
	// priority system?
	bool heavy;
};

// NOTE: remember to update this variable! heh.
constexpr int COMP_COUNT = 4;
}
}

