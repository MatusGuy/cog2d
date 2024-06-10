#include "actor.hpp"

Actor::Actor()
{
	Rect m_bbox;
	Vector m_vel;
}

void Actor::update()
{
	m_bbox.position.x += m_vel.x;
	m_bbox.position.y += m_vel.y;
}

void Actor::draw()
{
}
