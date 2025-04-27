#ifndef ACTORMANAGER_HPP
#define ACTORMANAGER_HPP

#include <vector>

#include "cog2d/scene/actor.hpp"

COG2D_NAMESPACE_BEGIN_DECL

/*!
 * \class ActorManager
 * \brief Controls all actors
 *
 * This class controls all \ref Actor instances.
 * They are updated & drawn by the \ref Program.
 */

class ActorManager
{
public:
	using Actors = std::vector<Actor*>;

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
		auto actor = new T(std::forward<Args>(args)...);
		add(actor);
		return actor;
	}

	void add(Actor* actor);

	/*!
	 * \return All actors in the manager.
	 */
	inline Actors& get_actors() { return m_actors; }

	void update();

private:
	Actors m_actors;
};

COG2D_NAMESPACE_END_DECL

#endif  // ACTORMANAGER_HPP
