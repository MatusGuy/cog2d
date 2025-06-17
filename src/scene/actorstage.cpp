// SPDX-License-Identifier: LGPL-3.0-only

#include "actorstage.hpp"

#include "cog2d/scene/scene.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

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
	m_current_scene->update(m_viewport);
}

void ActorStage::draw()
{
	m_current_scene->draw();
}

bool ActorStage::event(SDL_Event* ev)
{
	return true;
}

void ActorStage::set_current_scene(Scene* scene)
{
	m_current_scene = scene;
	m_current_scene->init();
}

COG2D_NAMESPACE_END_IMPL
