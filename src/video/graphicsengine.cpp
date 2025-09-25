// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "graphicsengine.hpp"

#include <optional>

#include "cog2d/program.hpp"

#include "cog2d/video/sdl2/sdl2graphicsengine.hpp"

namespace cog2d::graphics {

Backend::GraphicsEngine type = Backend::GRAPHICS_SDL;

Backend::Pixmap pixmap_backend;
Backend::Ttf ttf_backend;

#define COG2D_CALL_GRAPHICS(_f, ...) sdl::_f(__VA_ARGS__)
#define COG2D_CALL_RET_GRAPHICS(_f, ...) return sdl::_f(__VA_ARGS__)

void init(ProgramSettings& settings)
{
	COG2D_CALL_GRAPHICS(init, settings);
}

void deinit()
{
	COG2D_CALL_GRAPHICS(deinit);
}

void pre_draw()
{
	COG2D_CALL_GRAPHICS(pre_draw);
}

void post_draw()
{
	COG2D_CALL_GRAPHICS(post_draw);
}

void draw_rect(Rect rect, bool filled, Color color)
{
	COG2D_CALL_GRAPHICS(draw_rect, rect, filled, color);
}

void draw_circle(Vector center, float radius, bool filled, Color color)
{
	COG2D_CALL_GRAPHICS(draw_circle, center, radius, filled, color);
}

void draw_line(Vector a, Vector b)
{
	COG2D_CALL_GRAPHICS(draw_line, a, b);
}

void draw_point(Vector point)
{
	COG2D_CALL_GRAPHICS(draw_point, point);
}

void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color, Flip flip, float angle,
                  Vector center)
{
	COG2D_CALL_GRAPHICS(draw_texture, tex, src, dest, color, flip, angle, center);
}

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

Vector_t<int> get_logical_size()
{
	COG2D_CALL_RET_GRAPHICS(get_logical_size);
}

void push_target(Texture* tex)
{
	COG2D_CALL_GRAPHICS(push_target, tex);
}

void pop_target()
{
	COG2D_CALL_GRAPHICS(pop_target);
}

Texture* get_target()
{
	COG2D_CALL_RET_GRAPHICS(get_target);
}

}  //namespace cog2d::graphics
