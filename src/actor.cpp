#include "actor.hpp"

Actor::Actor():
	m_bbox{0,0,0,0},
	m_vel{0,0}
{

}

void Actor::update()
{
	m_bbox.x += m_vel.x;
	m_bbox.y += m_vel.y;
}

void Actor::draw()
{
}
