#include "actormanager.hpp"

#include "collisionsystem.hpp"
#include "logger.hpp"

ActorManager::ActorManager():
	m_actors()
{

}

void ActorManager::add(Actor* actor)
{
	m_actors.push_back(actor);
}

void ActorManager::update()
{
	for (int i = 0; i < m_actors.size(); i++) {
		Actor* actor = m_actors[i];
		if (!actor->is_active())
			continue;

		actor->update();
	}
}
