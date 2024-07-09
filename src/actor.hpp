#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "types.hpp"

/*!
 * \brief The Actor class
 *
 * The Actor class represents a moving object on a
 * scene.
 */
class Actor
{
public:
	Actor();

	/*!
	 * \brief update
	 *
	 * Updates the position of the actor for the next frame.
	 * This is called automatically by the \ref ActorManager
	 */
	virtual void update();
	virtual void draw() = 0;

protected:
	Rect m_bbox;
	Vector m_vel;
};

#endif // ACTOR_HPP
