// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/util/math/vector.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {

struct EntityBase
{
	std::uint16_t builtins = 0;

	/// Geometry
	Rect bbox;
	bool follow_camera = false;

	/// Velocity
	Vector vel;
};
}  //namespace cog2d
