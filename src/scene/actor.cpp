// SPDX-License-Identifier: LGPL-3.0-only

#include "actor.hpp"

#include "cog2d/scene/actormanager.hpp"
#include "cog2d/program.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

Actor::Actor(bool active)
    : CollisionBody(),
      m_vel(0, 0),
      m_accel(0, 0),
      m_grav(0),
      m_active(active),
      m_manager(nullptr)
{
}

void Actor::update()
{
	if (m_grav != 0)
		gravity();

	m_mov = m_vel;
}

void Actor::set_active(bool active)
{
	if (is_active() == active)
		return;

	m_active = active;
	m_manager->notify_activity(this);
}

void Actor::gravity()
{
	// This function hates operator overloads, so just add the x and y seperately.
	m_vel.x += m_accel.x * Program::get().m_delta_time;
	m_vel.y += (m_accel.y + m_grav) * Program::get().m_delta_time;
}

COG2D_NAMESPACE_END_IMPL
