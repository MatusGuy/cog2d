#include "collisionbody.hpp"


CollisionBody::CollisionBody():
	m_bbox(0, 0, 0, 0),
	m_movement(0, 0),
	m_last_collider(nullptr)
{

}

Rect CollisionBody::get_destination()
{
	Rect rect = m_bbox;
	rect.pos += m_movement;
	return rect;
}

void CollisionBody::apply_movement()
{
	m_bbox = get_destination();
	m_movement.x = 0;
	m_movement.y = 0;
	m_last_collider = nullptr;
}
