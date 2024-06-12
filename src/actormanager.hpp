#ifndef ACTORMANAGER_HPP
#define ACTORMANAGER_HPP

#include <vector>

#include "actor.hpp"

class ActorManager : public Singleton<ActorManager>
{
public:
	ActorManager();

	template<class T, typename... Args>
	T* create(Args&&... args)
	{
		auto actor = new T(std::forward<Args>(args)...);
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
