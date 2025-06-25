// SPDX-License-Identifier: LGPL-3.0-only

#include "actormanager.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/scene/actorrefsiterator.hpp"

namespace cog2d {

ActorManager::ActorManager()
    : m_actors()
{
}

void ActorManager::add(std::unique_ptr<Actor> actor)
{
	Actor* ptr = actor.get();
	m_actors.push_back(std::move(actor));

	ptr->m_manager = this;

	if (ptr->is_active()) {
		m_active_actors.push_back(ptr);
	}
}

void ActorManager::update()
{
	ActorRefsIterator it(m_active_actors.begin(), m_active_actors.begin());
	while (it.m_it != m_active_actors.end()) {
		Actor* actor = *it;
		//int idx = std::distance(m_active_actors.begin(), it);

		actor->update();

		//// Only advance if actor is active, because if otherwise, that means it has been removed
		//// from the active actors list, which would invalidate the iterator.
		//if (actor->is_active())
		//	it++;
		//else
		//	it = std::next(m_active_actors.begin(), idx);
		++it;
	}
}

void ActorManager::draw()
{
	for (auto it = m_active_actors.begin(); it != m_active_actors.end(); ++it) {
		Actor* actor = *it;
		actor->draw();
	}
}

void ActorManager::notify_activity(Actor* actor)
{
	if (actor->m_manager != this) {
		COG2D_LOG_WARN("ActorManager", "Attempted to perform operation on foreign actor.");
		return;
	}

	if (actor->is_active()) {
		m_active_actors.push_back(actor);
	} else {
		auto it = std::find(m_active_actors.begin(), m_active_actors.end(), actor);
		if (it == m_active_actors.end()) {
			// Huh, weird.
			return;
		}

		m_active_actors.erase(it);
	}
}

}
