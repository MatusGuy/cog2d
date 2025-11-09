// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <cinttypes>

#include "cog2d/util/math/vector.hpp"
#include "cog2d/util/math/rect.hpp"
#include "cog2d/ecs/entityid.hpp"

namespace cog2d {

enum ActiveState
{
	ACTIVE_MANUAL = 1 << 0,
	ACTIVE_VIEWPORT = 1 << 1
};

struct EntityBase
{
	EntityId id;
	std::uint16_t builtins = 0;

	std::uint8_t active = ACTIVE_MANUAL;

	Rect bbox = {};
	bool follow_camera = false;
	Vector viewport_pos();

	Vector vel;
};
}  //namespace cog2d
