// SPDX-License-Identifier: LGPL-3.0-only

#include "actorstage.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

ActorStage::ActorStage()
    : m_actormanager(),
      m_collisionsystem()
{
}

void ActorStage::init()
{
}

void ActorStage::update()
{
	m_actormanager.update();
	// FIXME: Is this a good way to handle this?
	m_collisionsystem.update(m_actormanager);
}

void ActorStage::draw()
{
	for (Actor* actor : m_actormanager.get_actors()) {
		if (!actor->is_active())
			continue;

		actor->draw();
	}
}

bool ActorStage::event(SDL_Event* ev)
{
	return true;
}

COG2D_NAMESPACE_END_IMPL
