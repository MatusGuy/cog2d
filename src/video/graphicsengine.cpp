// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "graphicsengine.hpp"

#include <optional>

#include "cog2d/program.hpp"

namespace cog2d {

void GraphicsEngine::draw_texture(Texture* tex, Rect dest, Color color, Flip flip, float angle,
                                  Vector center)
{
	draw_texture(tex, {{0, 0}, tex->size()}, dest, color, flip, angle, center);
}

void GraphicsEngine::draw_texture(Texture* tex, Vector pos, Color color, Flip flip, float angle,
                                  Vector center)
{
	draw_texture(tex, {pos, tex->size()}, color, flip, angle, center);
}

void GraphicsEngine::draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color, Flip flip,
                                  float angle)
{
	draw_texture(tex, src, dest, color, flip, angle, tex->size() / 2);
}

void GraphicsEngine::draw_texture(Texture* tex, Rect dest, Color color, Flip flip, float angle)
{
	draw_texture(tex, {{0, 0}, tex->size()}, dest, color, flip, angle, tex->size() / 2);
}

void GraphicsEngine::draw_texture(Texture* tex, Vector pos, Color color, Flip flip, float angle)
{
	draw_texture(tex, {pos, tex->size()}, color, flip, angle, tex->size() / 2);
}

void GraphicsEngine::push_target(Texture* tex)
{
	m_target_stack.push(tex);
}

void GraphicsEngine::pop_target()
{
	m_target_stack.pop();
}

}  //namespace cog2d
