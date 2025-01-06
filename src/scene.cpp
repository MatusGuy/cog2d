#include "scene.hpp"


Scene::Scene():
	m_actormanager(),
	m_collisionsystem()
{

}

void Scene::init()
{
	ActorManager::s_current = &m_actormanager;
	CollisionSystem::s_current = &m_collisionsystem;
}


void Scene::update()
{
	m_actormanager.update();
}

void Scene::draw()
{
	for (Actor* actor : m_actormanager.get_actors()) {
		if (!actor->is_active())
			continue;

		actor->draw();
	}
}

bool Scene::event(SDL_Event* ev)
{
	return true;
}
