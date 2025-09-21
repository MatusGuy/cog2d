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

struct ProgramSettings;

class SDL2GraphicsEngine : public GraphicsEngine
{
public:
	SDL2GraphicsEngine() {}

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	SDL_Texture* m_proxy = nullptr;

	std::stack<Color> m_color_stack;

public:
	void init(ProgramSettings* settings) override;
	void deinit() override;

	void pre_draw() override;
	void post_draw() override;

	void draw_rect(Rect rect, bool filled = false, Color color = 0xFFFFFFFF) override;
	void draw_circle(Vector center, float radius, bool filled = false,
	                 Color color = 0xFFFFFFFF) override;
	void draw_line(Vector a, Vector b, Color color = 0xFFFFFFFF) override;
	void draw_point(Vector point, Color color = 0xFFFFFFFF) override;

	void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color, Flip flip, float angle,
	                  Vector center) override;

	inline SDL_Window* get_window() { return m_window; }
	inline SDL_Renderer* get_renderer() { return m_renderer; }

	Color get_current_color();

	void push_target(Texture* tex) override;
	void pop_target() override;
};

}  // namespace cog2d
