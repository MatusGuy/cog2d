// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>

#include "cog2d/scene/collision/collisionsystem.hpp"
#include "cog2d/scene/actorcomponents.hpp"
#include "cog2d/util/typetraits.hpp"

#define COG2D_GET_COMPONENT(comp) \
	(static_cast<ActorComps::comp*>(m_comps[ActorComps::comp::type].get()))

namespace cog2d {

class ActorManager;

/*!
 * \brief A moving object on a scene
 *
 * It can be used to represent a player, an enemy, or
 * just a normal object in the game.
 */
class Actor
{
	friend class ActorManager;

public:
	ActorManager* m_manager;

	std::unique_ptr<ActorComps::Component> m_comps[ActorComps::COMP_COUNT];

public:
	Actor(bool active = true);

	template<class T, class = BaseOf<ActorComps::Component, T>>
	bool has_component()
	{
		return m_comps[T::type] != nullptr;
	}

	virtual void init() {}

	/*!
	 * Updates the position of the actor for the next frame.
	 * This is called automatically by the \ref ActorManager
	 */
	virtual void update();

	/*!
	 * Draws the actor in whatever way you'd like.
	 * This is called automatically by the \ref ActorManager
	 */
	virtual void draw() = 0;

	void set_active(bool active);
	inline bool is_active() { return m_active; }

	void gravity();

	Rect get_dest();
	void apply_movement();

	virtual CollisionSystem::Response collision(Actor* other);

	Vector get_draw_pos();

public:
	// The following functions crash the program if the component does not exist.
	// And I don't care.

	inline Rect& bbox() { return COG2D_GET_COMPONENT(Geometry)->bbox; }
	inline Vector& vel() { return COG2D_GET_COMPONENT(Velocity)->vel; }
	inline Vector& accel() { return COG2D_GET_COMPONENT(Gravity)->accel; }
	inline float& grav() { return COG2D_GET_COMPONENT(Gravity)->grav; }
	inline ActorComps::Collision& col() { return *COG2D_GET_COMPONENT(Collision); }

protected:
	template<class T, class = BaseOf<ActorComps::Component, T>>
	void add_component()
	{
		m_comps[T::type] = std::make_unique<T>();
		static_cast<T*>(m_comps[T::type].get())->setup(this);
	}

	virtual void add_components() = 0;

private:
	bool m_active;
};

}  //namespace cog2d
