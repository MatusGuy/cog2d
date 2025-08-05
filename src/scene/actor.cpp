// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "actor.hpp"

#include <chrono>

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
    : m_manual_active(active),
      m_manager(nullptr)
{
}

void Actor::update()
{
	if (!has_component<ActorComps::Geometry>())
		return;

	COG2D_USE_VIEWPORT;

	if (follow_camera())
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
	if (m_manual_active == active)
		return;

	bool old_active = is_active();
	m_manual_active = active;
	if (old_active == is_active())
		return;

	m_manager->notify_activity(this);
}

void Actor::set_viewport_active(bool active)
{
	if (m_viewport_active == active)
		return;

	bool old_active = is_active();
	m_viewport_active = active;
	if (old_active == is_active())
		return;

	m_manager->notify_activity(this);
}

void Actor::gravity()
{
	namespace ch = std::chrono;

	const ch::duration<float> dt_sec = Program::get().m_delta_time;
	vel().x += accel().x * dt_sec.count();
	vel().y += (accel().y + grav()) * dt_sec.count();
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

PropertyRefs Actor::properties()
{
	PropertyRefs out;

	if (has_component<ActorComps::Geometry>()) {
		out.push_back(&bbox().pos);
	}

	return out;
}

void Actor::apply_movement()
{
	bbox().pos = get_dest().pos;
	col().mov.x = 0;
	col().mov.y = 0;
}

template<typename T>
void Actor::set_property_p(int idx, T& value)
{
	PropertyRef& ref = properties()[idx];

	T* prop = std::get<T>(ref);

	if (prop != nullptr)
		// Woooahh!!! Calm down there buddy.
		return;

	(*prop) = value;
}

}  //namespace cog2d
