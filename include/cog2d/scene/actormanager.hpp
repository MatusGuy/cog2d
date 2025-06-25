// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>
#include <memory>
#include <list>
#include <algorithm>

#include "cog2d/scene/actor.hpp"
#include "cog2d/util/currenton.hpp"

namespace cog2d {

/*!
 * \class ActorManager
 * \brief Controls all actors
 *
 * This class controls all \ref Actor instances.
 * They are updated & drawn by the \ref Program.
 */

class ActorManager : public Currenton<ActorManager>
{
	friend class Actor;

public:
	using Actors = std::vector<std::unique_ptr<Actor>>;
	using ActorRefs = std::list<Actor*>;

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

	void update();
	void draw();

private:
	void notify_activity(Actor* actor);

private:
	Actors m_actors;
	ActorRefs m_active_actors;
};

}
