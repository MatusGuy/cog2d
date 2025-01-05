#include "actor.hpp"

Actor::Actor():
	CollisionBody(),
	m_vel(0, 0)
{
}

void Actor::update()
{
	m_mov = m_vel;
}
