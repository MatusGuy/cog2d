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

#define COG2D_USE_GRAPHICSENGINE COG2D_USING(GraphicsEngine, graphicsengine)
class GraphicsEngine : public Currenton<GraphicsEngine>, public Backend
{
public:
	enum Flip
	{
		FLIP_NONE = 0x0,
		FLIP_HORIZONTAL = 0x1,
		FLIP_VERTICAL = 0x2
	};

protected:
	Vector_t<int> m_logical_size;
	std::stack<Texture*> m_target_stack;

public:
	Backend::Pixmap m_pixmap_backend;
	Backend::Ttf m_ttf_backend;

	virtual void init(ProgramSettings* settings) = 0;
	virtual void deinit() = 0;

	virtual void pre_draw() = 0;
	virtual void post_draw() = 0;

	virtual void draw_rect(Rect rect, bool filled = false, Color color = 0xFFFFFFFF) = 0;
	virtual void draw_circle(Vector center, float radius, bool filled = false,
	                         Color color = 0xFFFFFFFF) = 0;
	virtual void draw_line(Vector a, Vector b, Color color = 0xFFFFFFFF) = 0;
	virtual void draw_point(Vector point, Color color = 0xFFFFFFFF) = 0;

	virtual void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color, Flip flip,
	                          float angle, Vector center) = 0;
	void draw_texture(Texture* tex, Rect dest, Color color, Flip flip, float angle, Vector center);
	void draw_texture(Texture* tex, Vector pos, Color color, Flip flip, float angle, Vector center);
	void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color = 0xFFFFFFFF,
	                  Flip flip = FLIP_NONE, float angle = 0.f);
	void draw_texture(Texture* tex, Rect dest, Color color = 0xFFFFFFFF, Flip flip = FLIP_NONE,
	                  float angle = 0.f);
	void draw_texture(Texture* tex, Vector pos, Color color = 0xFFFFFFFF, Flip flip = FLIP_NONE,
	                  float angle = 0.f);

	inline const Vector_t<int>& get_logical_size() { return m_logical_size; }

	virtual void push_target(Texture* tex);
	virtual void pop_target();
	inline Texture* get_target() { return m_target_stack.top(); }
};

}  // namespace cog2d
