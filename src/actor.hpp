#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "collisionbody.hpp"

/*!
 * \brief A moving object on a scene
 *
 * It can be used to represent a player, an enemy, or
 * just a normal object in the game.
 */
// TODO: Special type of actor without collision?
// "Why is this inheritly bad?"
// "Well, because, the diamond problem."
class Actor : public CollisionBody
{
public:
	Vector m_vel;

public:
	Actor();

	/*!
	 * Updates the position of the actor for the next frame.
	 * This is called automatically by the \ref ActorManager
	 */
	virtual void update();

	/*!
	 * Draws the actor in whatever way you'd like.
	 * This is called automatically by the \ref ActorManager
	 */
	virtual void draw() = 0;

	virtual bool is_active() { return true; }
};

#endif // ACTOR_HPP
