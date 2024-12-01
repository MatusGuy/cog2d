#include "actormanager.hpp"

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
	for (Actor* actor : get_actors()) {
		if (!actor->is_active())
			continue;

		actor->update();
	}
}
