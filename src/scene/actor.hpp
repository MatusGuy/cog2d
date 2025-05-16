// SPDX-License-Identifier: LGPL-3.0-only

// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/scene/collision/collisionbody.hpp"

COG2D_NAMESPACE_BEGIN_DECL

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
	Vector m_accel;
	float m_grav;

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

	void gravity();
};

COG2D_NAMESPACE_END_DECL
