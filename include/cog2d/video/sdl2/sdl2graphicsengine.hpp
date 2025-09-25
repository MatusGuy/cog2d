// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <memory>
#include <string>
#include <stack>

#include "cog2d/video/graphicsengine.hpp"

#include "cog2d/video/sdl2/sdl2texture.hpp"

namespace cog2d {
class ProgramSettings;
namespace graphics {
namespace sdl {

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

SDL_Window* get_window();
SDL_Renderer* get_renderer();

Vector_t<int> get_logical_size();

void push_target(Texture* tex);
void pop_target();
Texture* get_target();

}  //namespace sdl
}  //namespace graphics
}  //namespace cog2d
