#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <vector>

#include "collisionbody.hpp"

#define COG2D_USE_COLLISIONSYSTEM COG2D_USING(CollisionSystem, collisionsystem)
class CollisionSystem : public Singleton<CollisionSystem> {
public:
	CollisionSystem();

	void update();

	void rect_rect(CollisionBody* a, CollisionBody* b);

};

#endif // COLLISIONSYSTEM_HPP
