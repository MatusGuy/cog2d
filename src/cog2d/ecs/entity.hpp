// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <cstdint>
#include <cstddef>

#include "cog2d/util/math/vector.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {
struct CompCollision
{
	Vector mov;
	std::uint32_t group;

	/// static and immovable
	bool stabile;
};

struct CompGravity
{
	Vector accel;

	/// gravitational acceleration
	float grav = 1;
};

struct EntityBase
{
	std::uint64_t comps;

	/// Geometry
	Rect bbox;
	bool follow_camera = false;

	/// Velocity
	Vector vel;
};
}  //namespace cog2d
