// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "entitybase.hpp"

#include "cog2d/ecs/world.hpp"

namespace cog2d {
Vector EntityBase::viewport_pos() {
	return bbox.pos - ext::viewport.region.pos;
}
}
