// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "graphicsengine.hpp"

namespace cog2d::graphics {

Backend::GraphicsEngine type = Backend::GRAPHICS_SDL;

Backend::Pixmap pixmap_backend;
Backend::Ttf ttf_backend;

void draw_texture(Texture* tex, Rect dest, Color color, Flip flip, float angle, Vector center)
{
	draw_texture(tex, {{0, 0}, tex->size()}, dest, color, flip, angle, center);
}

void draw_texture(Texture* tex, Vector pos, Color color, Flip flip, float angle, Vector center)
{
	draw_texture(tex, {pos, tex->size()}, color, flip, angle, center);
}

void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color, Flip flip, float angle)
{
	draw_texture(tex, src, dest, color, flip, angle, tex->size() / 2);
}

void draw_texture(Texture* tex, Rect dest, Color color, Flip flip, float angle)
{
	draw_texture(tex, {{0, 0}, tex->size()}, dest, color, flip, angle, tex->size() / 2);
}

void draw_texture(Texture* tex, Vector pos, Color color, Flip flip, float angle)
{
	draw_texture(tex, {pos, tex->size()}, color, flip, angle, tex->size() / 2);
}

}  //namespace cog2d::graphics
