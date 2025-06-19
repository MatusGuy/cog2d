// SPDX-License-Identifier: LGPL-3.0-only

#include "scene.hpp"

#include "cog2d/scene/viewport.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

Scene::Scene()
{

}

void Scene::update()
{
	prepare_context();

	m_actormanager.update();
	m_collisionsystem.update(m_actormanager);
}

void Scene::draw()
{
	prepare_context();

	m_actormanager.draw();
}

void Scene::prepare_context()
{
	ActorManager::s_current = &m_actormanager;
	CollisionSystem::s_current = &m_collisionsystem;
}

COG2D_NAMESPACE_END_IMPL
