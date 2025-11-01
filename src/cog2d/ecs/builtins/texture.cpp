// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only

#include "texture.hpp"

#include "cog2d/ecs/entity.hpp"

namespace cog2d::systems {

void texture_draw(EntityBase& ent, CompTexture& tex)
{
	if (tex.texdata == nullptr)
		graphics::draw_rect(ent.bbox, false, tex.color);
	else
		graphics::draw_texture(tex.texdata, ent.bbox.pos, tex.color, tex.flip, tex.angle);
}

}  //namespace cog2d::systems
