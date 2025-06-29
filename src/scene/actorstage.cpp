// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "actorstage.hpp"

#include "cog2d/scene/scene.hpp"

namespace cog2d {

ActorStage::ActorStage()
    : m_current_scene(nullptr),
      m_viewport()
{
}

void ActorStage::init()
{
}

void ActorStage::update()
{
	Viewport::s_current = &m_viewport;
	m_current_scene->update();
	m_viewport.get_camera()->update();
}

void ActorStage::draw()
{
	Viewport::s_current = &m_viewport;
	m_current_scene->draw();
}

bool ActorStage::event(SDL_Event* ev)
{
	return true;
}

void ActorStage::set_current_scene(Scene* scene)
{
	if (m_current_scene)
		m_current_scene->m_stage = nullptr;

	m_current_scene = scene;

	Viewport::s_current = &m_viewport;
	m_current_scene->m_stage = this;
	m_current_scene->init();
}

}
