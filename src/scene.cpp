#include "scene.hpp"

Scene::Scene()
    : m_actormanager(),
      m_collisionsystem()
{
}

void Scene::init()
{
}

void Scene::update()
{
	m_actormanager.update();
	// FIXME: Is this a good way to handle this?
	m_collisionsystem.update(m_actormanager);
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
