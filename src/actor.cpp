#include "actor.hpp"

Actor::Actor():
	m_bbox(0, 0, 0, 0),
	m_vel(0, 0)
{
}

void Actor::update()
{
	m_bbox.pos += m_vel;
}
