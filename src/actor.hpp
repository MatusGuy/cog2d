#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "types.hpp"

class Actor
{
public:
	Actor();

	virtual void update();
	virtual void draw();

protected:
	Rect m_bbox;
	Vector m_vel;
};

#endif // ACTOR_HPP
