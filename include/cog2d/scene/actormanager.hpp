// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>
#include <memory>
#include <list>
#include <algorithm>

#include "cog2d/scene/actor.hpp"
#include "cog2d/util/currenton.hpp"
#include "cog2d/scene/collision/collisionsystem.hpp"
#include "cog2d/scene/actorcontainers.hpp"

namespace cog2d {

/*!
 * \class ActorManager
 * \brief Controls all actors
 *
 * This class controls all \ref Actor instances.
 * They are updated & drawn by the \ref Program.
 */
#define COG2D_USE_ACTORMANAGER COG2D_USING(ActorManager, actormanager)
class ActorManager : public Currenton<ActorManager>
{
	friend class Actor;

public:
	ActorManager();

	/*!
	 * This function creates an \ref Actor and adds it immediately to
	 * the \ref ActorManager.
	 *
	 * \param args Arguments passed to the constructor
	 * \return Pointer to the fresh new \ref Actor
	 */
	template<class T, typename... Args>
	T* create(Args&&... args)
	{
		auto actor = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = actor.get();
		add(std::move(actor));
		return ptr;
	}

	void add(std::unique_ptr<Actor> actor);

	/*!
	 * \return All actors in the manager.
	 */
	inline ActorRefs& get_active_actors() { return m_active_actors; }

	inline CollisionSystem& colsystem() { return m_collisionsystem; }

	void update();
	void draw();

private:
	void notify_activity(Actor* actor);

	void update_actor(Actor* actor);

private:
	Actors m_actors;
	ActorRefs m_active_actors;
	CollisionSystem m_collisionsystem;
};

}
