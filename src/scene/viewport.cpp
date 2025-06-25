// SPDX-License-Identifier: LGPL-3.0-only

#include "viewport.hpp"

#include "cog2d/video/graphicsengine.hpp"

namespace cog2d {

Viewport::Viewport()
    : m_region(0, 0, 0, 0),
      m_camera(nullptr)
{
	COG2D_USE_GRAPHICSENGINE;
	m_region.size = graphicsengine.get_logical_size();
}

Viewport::Viewport(Rect_t<int> region)
    : m_region(region),
      m_camera(nullptr)
{
}

void Viewport::set_camera(Camera* camera)
{
	m_camera = camera;
}

}
