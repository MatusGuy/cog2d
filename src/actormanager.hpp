#ifndef ACTORMANAGER_HPP
#define ACTORMANAGER_HPP

#include <vector>

#include "actor.hpp"


/*!
 * \class ActorManager
 * \brief Controls all actors
 *
 * This class controls all \ref Actor instances.
 * They are updated & drawn by the \ref Program.
 */
#define COG2D_USE_ACTORMANAGER COG2D_USING(ActorManager, actormanager)
class ActorManager : public Singleton<ActorManager>
{
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
	inline std::vector<Actor*>& get_actors() {
		return m_actors;
	}

	void update();

private:
	std::vector<Actor*> m_actors;
};

#endif // ACTORMANAGER_HPP
