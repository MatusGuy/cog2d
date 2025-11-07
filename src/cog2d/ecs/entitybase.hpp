// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <cinttypes>

#include "cog2d/util/math/vector.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {

using EntityId = std::uint32_t;

struct EntityBase
{
	EntityId id;
	std::uint16_t builtins = 0;

	Rect bbox;
	bool follow_camera = false;
	Vector viewport_pos();

	Vector vel;
};
}  //namespace cog2d
