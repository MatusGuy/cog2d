// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <variant>

#include "cog2d/scene/collision/collisionsystem.hpp"
#include "cog2d/scene/actorcomponents.hpp"
#include "cog2d/util/typetraits.hpp"
#include "cog2d/util/logger.hpp"
#include "cog2d/video/color.hpp"

#define COG2D_GET_COMPONENT(comp) \
	(static_cast<ActorComps::comp*>(m_comps[ActorComps::comp::type].get()))

#ifndef COG2D_ACTOR_TYPES
#define COG2D_ACTOR(clname)           \
public:                               \
	static std::string classname_s()  \
    {                                 \
	    return #clname;               \
	}                                 \
	std::string classname() override  \
    {                                 \
	    return classname_s();         \
	}                                 \
	static std::uint16_t classidx_s() \
    {                                 \
	    return 0;                     \
	}                                 \
	std::uint16_t classidx() override \
    {                                 \
	    return classidx_s();          \
	}                                 \
	                                  \
private:
#else
#define COG2D_ACTOR(clname)                \
public:                                    \
	static std::string classname_s()       \
    {                                      \
	    return #clname;                    \
	}                                      \
	std::string classname() override       \
    {                                      \
	    return classname_s();              \
	}                                      \
	static std::uint16_t classidx_s()      \
    {                                      \
	    return ::cog2d::ActorType::clname; \
	}                                      \
	std::uint16_t classidx() override      \
    {                                      \
	    return ::cog2d::ActorType::clname; \
	}                                      \
	                                       \
private:
#endif

namespace cog2d {

enum PropertyType : std::uint8_t
{
	PROPTYPE_NONE,
	PROPTYPE_INT,
	PROPTYPE_BOOL,
	PROPTYPE_FLOAT,
	PROPTYPE_STRING,
	PROPTYPE_VECTOR,
	PROPTYPE_VECTORI,
	PROPTYPE_RECT,
	PROPTYPE_RECTI,
	PROPTYPE_COLOR
};

// Make sure to add new types to this messy list
template<typename T>
inline constexpr bool is_property_value()
{
	return std::is_same_v<T, std::int32_t> || std::is_same_v<T, bool> || std::is_same_v<T, float> ||
	       std::is_same_v<T, std::string> || std::is_same_v<T, Vector> ||
	       std::is_same_v<T, Vector_t<std::int32_t>> || std::is_same_v<T, Rect> ||
	       std::is_same_v<T, Rect_t<std::int32_t>> || std::is_same_v<T, Color>;
}

using PropertyRef = std::variant<std::int32_t*, bool*, float*, std::string*, Vector*,
                                 Vector_t<std::int32_t>*, Rect*, Rect_t<std::int32_t>*, Color*>;
using PropertyRefs = std::vector<PropertyRef>;

class ActorManager;
class Actor;

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
	static std::string classname_s() { return ""; }
	virtual std::string classname() { return classname_s(); }
	static std::uint16_t classidx_s() { return 0; }
	virtual std::uint16_t classidx() { return classidx_s(); }

	virtual PropertyRefs properties();

	template<typename T, typename = std::enable_if<is_property_value<T>(), T>>
	inline void set_property(int idx, T& value)
	{
		PropertyRef ref = properties().at(idx);

		T* prop = std::get<T*>(ref);

		if (prop == nullptr)
			// TODO: Throw exception
			return;

		(*prop) = value;
	}

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

	template<typename T>
	void set_property_p(int idx, T& value);
};

}  //namespace cog2d
