#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <vector>

#include "collisionbody.hpp"

#define COG2D_USE_COLLISIONSYSTEM COG2D_USING(CollisionSystem, collisionsystem)
class CollisionSystem : public Singleton<CollisionSystem> {
public:
	/// @brief A collision group.
	// std::vector has a specializaion for bool that
	// tries to use bits for individual elements, so
	// no need to worry about space here.
	// FIXME: does this really need to be dynamic?
	using Group = std::vector<bool>;

public:
	std::vector<Group> m_groups;

public:
	CollisionSystem();

	void update();

	void rect_rect(CollisionBody* a, CollisionBody* b);

};

#endif // COLLISIONSYSTEM_HPP
