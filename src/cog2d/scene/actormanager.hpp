// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>
#include <memory>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <forward_list>

#include "cog2d/scene/actor.hpp"
#include "cog2d/util/currenton.hpp"
#include "cog2d/scene/collision/collisionsystem.hpp"
#include "cog2d/scene/actorcontainers.hpp"
#include "cog2d/scene/actorfactory.hpp"

namespace cog2d {

#define COG2D_USE_ACTORMANAGER COG2D_USING(ActorManager, actormanager)
class ActorManager : public Currenton<ActorManager>
{
	friend class Actor;

public:
	ActorManager(ActorFactory* factory = nullptr);

	template<class T, typename... Args>
	T* create(Args&&... args)
	{
		auto actor = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = actor.get();
		add(std::move(actor));
		return ptr;
	}

	void add(std::unique_ptr<Actor> actor);

	inline ActorRefs& get_active_actors() { return m_active_actors; }

	inline CollisionSystem& colsystem() { return m_collisionsystem; }

	inline ActorFactory* factory() { return m_factory; }
	inline void set_factory(ActorFactory* f)
	{
		m_factory = f;
		f->m_actormanager = this;
	}

	void update();
	void draw();

	template<class T, class = BaseOf<Actor, T>>
	inline void allow_type_indexing()
	{
		m_actors_by_class_idx.insert({T::classidx_s(), {}});
	}

	template<class T, class = BaseOf<Actor, T>>
	inline void allow_active_type_indexing()
	{
		m_active_actors_by_class_idx.insert({T::classidx_s(), {}});
	}

	template<class T, class = BaseOf<Actor, T>>
	inline ActorRefs& get_active_actors_of_type()
	{
		return m_active_actors_by_class_idx[T::classidx_s()];
	}

private:
	void notify_activity(Actor* actor);

	void update_actor(Actor* actor);

private:
	Actors m_actors;
	ActorRefs m_active_actors;
	ActorFactory* m_factory;
	CollisionSystem m_collisionsystem;

	std::unordered_map<std::uint16_t, ActorRefs> m_actors_by_class_idx;
	std::unordered_map<std::uint16_t, ActorRefs> m_active_actors_by_class_idx;
};

}  //namespace cog2d
