#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "types.hpp"

/*!
 * \brief A moving object on a scene
 *
 * It can be used to represent a player, an enemy, or
 * just a normal object in your game.
 */
class Actor
{
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

	/*!
	 * \return The bounding box
	 */
	inline Rect& get_bbox() { return m_bbox; }

	/*!
	 * \return The bounding vel
	 */
	inline Vector& get_vel() { return m_vel; }

protected:
	Rect m_bbox;
	Vector m_vel;
};

#endif // ACTOR_HPP
