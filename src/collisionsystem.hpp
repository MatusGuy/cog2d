#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <vector>

#include "actormanager.hpp"
#include "types.hpp"

class CollisionBody;

class CollisionSystem
{
public:
	/// @brief A collision group.
	// std::vector has a specializaion for bool that
	// tries to use bits for individual elements, so
	// no need to worry about space here.
	// FIXME: does this really need to be dynamic?
	using Group = std::vector<bool>;

	enum Response : std::uint8_t {
		COLRESP_ACCEPT,
		COLRESP_REJECT
	};

public:
	std::vector<Group> m_groups;

public:
	CollisionSystem();

    void update(ActorManager &actormanager);

    void rect_rect(CollisionBody *a, CollisionBody *b);
};

#endif // COLLISIONSYSTEM_HPP
