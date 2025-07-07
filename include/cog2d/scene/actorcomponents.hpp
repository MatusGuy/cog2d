// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/math/rect.hpp"
#include "cog2d/util/typetraits.hpp"

#define COG2D_ACTOR_COMPONENT(t) constexpr static std::uint8_t type = t;
#define COG2D_ACTOR_COMP_DEPEND(comp, actor)                 \
	if (!actor->has_component<comp>()) {                     \
		constexpr auto msg =                                 \
		    "Actor has no component of type {}. "            \
		    "It is depended on by component of type {}.";    \
		COG2D_LOG_ERROR(fmt::format(msg, comp::type, type)); \
	}

namespace cog2d {

class Actor;

namespace ActorComps {

struct Component
{
	/// Check component dependencies and stuff
	void setup(Actor* actor) {}
};

struct Geometry : public Component, public Rect
{
	COG2D_ACTOR_COMPONENT(0)

	Rect bbox;
	bool follow_camera = false;
};

struct Velocity : public Component
{
	COG2D_ACTOR_COMPONENT(1)

	Vector vel;

	void setup(Actor* actor);
};

struct Gravity : public Component
{
	COG2D_ACTOR_COMPONENT(2)

	Vector accel;

	/// Gravity modifier
	float grav;

	void setup(Actor* actor);
};

struct Collision : public Component
{
	COG2D_ACTOR_COMPONENT(3)

	Vector mov;
	int group;

	// TODO: improve upon this - maybe make some sort of
	// priority system?
	bool heavy;

	void setup(Actor* actor);
};

// NOTE: remember to update this variable! heh.
constexpr int COMP_COUNT = 4;
}
}  //namespace cog2d
