// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "actor.hpp"

#include "cog2d/scene/actormanager.hpp"
#include "cog2d/program.hpp"
#include "cog2d/util/logger.hpp"
#include "cog2d/scene/viewport.hpp"

namespace cog2d {

namespace ActorComps {
void Velocity::setup(Actor* actor)
{
	COG2D_ACTOR_COMP_DEPEND(Geometry, actor)
}

void Gravity::setup(Actor* actor)
{
	COG2D_ACTOR_COMP_DEPEND(Velocity, actor)
}

void Collision::setup(Actor* actor)
{
	COG2D_ACTOR_COMP_DEPEND(Geometry, actor)
}
}  //namespace ActorComps

Actor::Actor(bool active)
    : m_active(active),
      m_manager(nullptr)
{
}

void Actor::update()
{
	if (!has_component<ActorComps::Geometry>())
		return;

	COG2D_USE_VIEWPORT;

	if (COG2D_GET_COMPONENT(Geometry)->follow_camera)
		bbox().pos += viewport.get_camera()->m_delta;

	if (!has_component<ActorComps::Velocity>())
		return;

	if (has_component<ActorComps::Gravity>())
		gravity();

	if (has_component<ActorComps::Collision>())
		col().mov = vel();
	else
		bbox().pos += vel();
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
	vel().x += accel().x * Program::get().m_delta_time;
	vel().y += (accel().y + grav()) * Program::get().m_delta_time;
}

Rect Actor::get_dest()
{
	Rect rect = bbox();
	rect.pos += col().mov;
	return rect;
}

CollisionSystem::Response Actor::collision(Actor*)
{
	return CollisionSystem::COLRESP_ACCEPT;
}

Vector Actor::viewport_pos()
{
	return bbox().pos - Viewport::get().get_camera()->m_pos;
}

void Actor::apply_movement()
{
	bbox().pos = get_dest().pos;
	col().mov.x = 0;
	col().mov.y = 0;
}
}
