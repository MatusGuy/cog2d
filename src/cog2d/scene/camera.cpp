// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "camera.hpp"

namespace cog2d {

Camera::Camera()
{

}

void Camera::pre_update()
{
	m_oldpos = m_pos;
}

void Camera::post_update()
{
	m_delta = m_pos - m_oldpos;
}
}
