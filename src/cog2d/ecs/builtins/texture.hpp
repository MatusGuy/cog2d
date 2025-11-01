// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/video/graphicsengine.hpp"

namespace cog2d {

struct EntityBase;
struct CompTexture
{
	cog2d::Texture* texdata = nullptr;

	Color color = 0xFFFFFFFF;
	float angle = 0.f;
	graphics::Flip flip = graphics::FLIP_NONE;
};

namespace systems {
void texture_draw(EntityBase& ent, CompTexture& tex);
}
}  //namespace cog2d
