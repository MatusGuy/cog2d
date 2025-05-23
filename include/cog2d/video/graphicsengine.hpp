// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>
#include <cmath>
#include <memory>
#include <SDL2/SDL.h>

#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/video/font/font.hpp"
#include "cog2d/video/color.hpp"
#include "cog2d/util/math/rect.hpp"

COG2D_NAMESPACE_BEGIN_DECL

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

public:
	void init(ProgramSettings* settings);
	void deinit();

	void pre_draw();
	void post_draw();

	void draw_rect(Rect rect, bool filled = false, Color color = 0xFFFFFFFF);
	void draw_circle(Vector center, float radius, bool filled = false, Color color = 0xFFFFFFFF);
	void draw_line(Vector a, Vector b, Color color = 0xFFFFFFFF);
	void draw_point(Vector point, Color color = 0xFFFFFFFF);
	void draw_texture(Rect dest, Texture* tex);
	void draw_texture(Rect dest, Texture* tex, float angle, SDL_RendererFlip flip = SDL_FLIP_NONE);

	inline SDL_Window* get_window() { return m_window; }
	inline SDL_Renderer* get_renderer() { return m_renderer; }

	Color get_current_color();

	inline const std::string& get_error() { return m_error; }
	inline const Vector_t<int>& get_logical_size() { return m_logical_size; }
};

COG2D_NAMESPACE_END_DECL
