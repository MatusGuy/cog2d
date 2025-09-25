// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <memory>
#include <string>
#include <stack>

#include "cog2d/util/currenton.hpp"
#include "cog2d/util/math/rect.hpp"
#include "cog2d/video/color.hpp"
#include "cog2d/video/texture.hpp"
#include "cog2d/util/backend.hpp"

namespace cog2d {
struct ProgramSettings;
namespace graphics {

extern Backend::GraphicsEngine type;

enum Flip
{
	FLIP_NONE = 0x0,
	FLIP_HORIZONTAL = 0x1,
	FLIP_VERTICAL = 0x2
};

extern Backend::Pixmap pixmap_backend;
extern Backend::Ttf ttf_backend;

void init(ProgramSettings& settings);
void deinit();

void pre_draw();
void post_draw();

void draw_rect(Rect rect, bool filled = false, Color color = 0xFFFFFFFF);
void draw_circle(Vector center, float radius, bool filled = false, Color color = 0xFFFFFFFF);
void draw_line(Vector a, Vector b, Color color = 0xFFFFFFFF);
void draw_point(Vector point, Color color = 0xFFFFFFFF);

void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color, Flip flip, float angle,
                  Vector center);
void draw_texture(Texture* tex, Rect dest, Color color, Flip flip, float angle, Vector center);
void draw_texture(Texture* tex, Vector pos, Color color, Flip flip, float angle, Vector center);
void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color = 0xFFFFFFFF,
                  Flip flip = FLIP_NONE, float angle = 0.f);
void draw_texture(Texture* tex, Rect dest, Color color = 0xFFFFFFFF, Flip flip = FLIP_NONE,
                  float angle = 0.f);
void draw_texture(Texture* tex, Vector pos, Color color = 0xFFFFFFFF, Flip flip = FLIP_NONE,
                  float angle = 0.f);

Vector_t<int> get_logical_size();

void push_target(Texture* tex);
void pop_target();
Texture* get_target();

}  //namespace graphics
}  //namespace cog2d
