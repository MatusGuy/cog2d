// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "actormanager.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/scene/actorrefsiterator.hpp"
#include "cog2d/scene/viewport.hpp"

namespace cog2d {

ActorManager::ActorManager(ActorFactory* factory)
	: m_actors(),
	  m_active_actors(),
	  m_factory(factory),
	  m_collisionsystem()
{
	m_collisionsystem.m_manager = this;
}

void ActorManager::add(std::unique_ptr<Actor> actor)
{
	Actor* ptr = actor.get();

	ptr->m_manager = this;
	ptr->add_components();

	m_actors.push_back(std::move(actor));

	ptr->init();

	if (ptr->is_active()) {
		notify_activity(ptr);
	}
}

void ActorManager::update()
{
	COG2D_USE_VIEWPORT;

	/*
	ActorRefsIterator it(m_active_actors.begin(), m_active_actors.begin());
	while (it.m_it != m_active_actors.end()) {
	    Actor* actor = *it;
	    //int idx = std::distance(m_active_actors.begin(), it);

	    actor->update();

	    ++it;
	}
	*/

	Rect viewportrect = Rect(viewport.get_camera()->m_pos, viewport.m_region.size).grown(32.f);

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it) {
		Actor* actor = (*it).get();

		if (actor->has_component<ActorComps::Geometry>())
			actor->set_viewport_active(viewportrect.overlaps(actor->bbox()));

		if (actor->is_active())
			actor->update();
	}

	m_collisionsystem.update();
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
		if (actor->has_component<ActorComps::Collision>()) {
			m_collisionsystem.m_actors.push_back(actor);
		}
	} else {
		// FIXME: theres no need to make this O(2n)...

		ActorRefs::iterator it = std::find(m_active_actors.begin(), m_active_actors.end(), actor);
		if (it == m_active_actors.end()) {
			// Huh, weird.
			return;
		}

		m_active_actors.erase(it);

		if (actor->has_component<ActorComps::Collision>()) {
			it = std::find(m_collisionsystem.m_actors.begin(), m_collisionsystem.m_actors.end(),
			               actor);
			if (it == m_collisionsystem.m_actors.end()) {
				// Huh, weird.
				return;
			}

			m_collisionsystem.m_actors.erase(it);
		}
	}
}
}  //namespace cog2d
