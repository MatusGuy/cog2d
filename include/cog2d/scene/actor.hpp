// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <variant>

#include "cog2d/scene/collision/collisionsystem.hpp"
#include "cog2d/scene/actorcomponents.hpp"
#include "cog2d/util/typetraits.hpp"
#include "cog2d/video/color.hpp"

#define COG2D_GET_COMPONENT(comp) \
	(static_cast<ActorComps::comp*>(m_comps[ActorComps::comp::type].get()))

namespace cog2d {

// TODO: Clang-format: This should be aligned with spaces, not tabs
using PropertyRef = std::variant<std::int32_t*, bool*, float*, std::string*, Vector*,
								 Vector_t<std::int32_t>*, Rect*, Rect_t<std::int32_t>*, Color*>;
using PropertyRefs = std::vector<PropertyRef>;

class ActorManager;
class Actor;

template<class A, class = BaseOf<A, Actor>, typename T>
void set_property(A& actor, std::size_t idx, T value);

template<class A, class = BaseOf<A, Actor>, typename T>
const PropertyRef get_property(A& actor, std::size_t idx);

/**
 * @brief A moving object on a scene
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
	virtual void update();
	virtual void draw() = 0;

	void set_active(bool active);
	inline bool is_active() { return m_manual_active && m_viewport_active; }

	void gravity();

	Rect get_dest();
	void apply_movement();

	virtual CollisionSystem::Response collision(Actor* other);

	Vector viewport_pos();

public:
	static std::string classname() { return ""; }
	static PropertyRefs properties() { return {}; }

public:
	// The following functions crash the program if the component does not exist.
	// And I don't care.

	inline Rect& bbox() { return COG2D_GET_COMPONENT(Geometry)->bbox; }
	inline bool& follow_camera() { return COG2D_GET_COMPONENT(Geometry)->follow_camera; }
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
	/// True if actor wants to be active
	bool m_manual_active;

	/// True if actor was activated because it entered the camera region
	bool m_viewport_active;

	void set_viewport_active(bool active);
};

}  //namespace cog2d
