// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/math/rect.hpp"
#include "cog2d/scene/camera.hpp"
#include "cog2d/util/currenton.hpp"

namespace cog2d {

#define COG2D_USE_VIEWPORT COG2D_USING(Viewport, viewport)
#define COG2D_USE_CAMERA cog2d::Camera& camera = Viewport::get().get_camera()
class Viewport : public Currenton<Viewport>
{
	friend class Camera;

public:
	Rect_t<int> m_region;

public:
	Viewport();
	Viewport(Rect_t<int> region);

	void set_camera(Camera* camera);
	inline Camera* get_camera() { return m_camera; }

	inline Rect get_scene_rect()
	{
		return Rect(get_camera() ? get_camera()->m_pos : static_cast<Vector>(m_region.pos),
		            static_cast<Vector>(m_region.size));
	}

private:
	Camera* m_camera;
};

}
