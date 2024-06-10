#ifndef ACTORMANAGER_HPP
#define ACTORMANAGER_HPP

#include <vector>

#include "actor.hpp"

class ActorManager : public Singleton<ActorManager>
{
public:
	ActorManager();

	template<class T>
	T* create()
	{
		auto actor = new T;
		m_actors.push_back(actor);
		return actor;
	}

	inline const std::vector<Actor*>& get_actors() const {
		return m_actors;
	}

private:
	std::vector<Actor*> m_actors;
};

#endif // ACTORMANAGER_HPP
