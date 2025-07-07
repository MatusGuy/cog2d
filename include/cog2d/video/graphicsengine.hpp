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
#include "cog2d/video/font/font.hpp"
#include "cog2d/video/texture.hpp"

namespace cog2d {

struct ProgramSettings;

#define COG2D_USE_GRAPHICSENGINE COG2D_USING(GraphicsEngine, graphicsengine)
class GraphicsEngine : public Currenton<GraphicsEngine>
{
private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	SDL_Texture* m_proxy = nullptr;

	std::string m_error = "";

	Vector_t<int> m_logical_size;

	std::stack<Texture*> m_target_stack;
	std::stack<Color> m_color_stack;

public:
	void init(ProgramSettings* settings);
	void deinit();

	void pre_draw();
	void post_draw();

	void draw_rect(Rect rect, bool filled = false, Color color = 0xFFFFFFFF);
	void draw_circle(Vector center, float radius, bool filled = false, Color color = 0xFFFFFFFF);
	void draw_line(Vector a, Vector b, Color color = 0xFFFFFFFF);
	void draw_point(Vector point, Color color = 0xFFFFFFFF);

	// TODO: what if center really should be {-1, -1}?? Uhh.......
	void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color = 0xFFFFFFFF,
	                  float angle = 0.f, Vector center = {-1, -1},
	                  SDL_RendererFlip flip = SDL_FLIP_NONE);
	void draw_texture(Texture* tex, Rect dest, Color color = 0xFFFFFFFF, float angle = 0,
	                  Vector center = {-1, -1}, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void draw_texture(Texture* tex, Vector pos, Color color = 0xFFFFFFFF, float angle = 0,
	                  Vector center = {-1, -1}, SDL_RendererFlip flip = SDL_FLIP_NONE);

	inline SDL_Window* get_window() { return m_window; }
	inline SDL_Renderer* get_renderer() { return m_renderer; }

	Color get_current_color();

	inline const std::string& get_error() { return m_error; }
	inline const Vector_t<int>& get_logical_size() { return m_logical_size; }

	void push_target(Texture* tex);
	void pop_target();
	inline Texture* get_target() { return m_target_stack.top(); }
};

}  // namespace cog2d
