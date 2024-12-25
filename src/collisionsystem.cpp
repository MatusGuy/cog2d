#include "collisionsystem.hpp"

#include "actormanager.hpp"

CollisionSystem::CollisionSystem()
{

}

void CollisionSystem::update() {
	COG2D_USE_ACTORMANAGER;

	ActorManager::Actors& actors = actormanager.get_actors();

	// iterator for inner for loop
	ActorManager::Actors::iterator iter;

	for (Actor* a : actors) {
		for (iter = actors.begin() + 1; iter != actors.end(); iter++) {
			Actor* b = *iter;
			if (!a->is_active() || !b->is_active() || a == b)
				continue;

			rect_rect(a, b);
		}
	}

	for (Actor* actor : actors) {
		actor->apply_movement();
	}
}

void CollisionSystem::rect_rect(CollisionBody* a, CollisionBody* b)
{
	Rect& r1 = a->m_bbox;
	Rect& r2 = b->m_bbox;
	Rect dest = a->get_destination();
	Vector& mov = a->m_movement;

	// TODO: in_bounds function

	if (!r1.overlaps(r2))
		return;

	bool hi = r1.get_right() > r2.get_left() && r2.get_right() > r1.get_left();
	bool vi = r1.get_bottom() > r2.get_top() && r2.get_bottom() > r1.get_top();

	if (dest.get_bottom() > r2.get_top() && hi)
	{
		b->m_movement.y += mov.y;
	}

	if (dest.get_top() > r2.get_bottom() && hi)
	{
		b->m_movement.y -= mov.y;
	}

	if (dest.get_right() > r2.get_left() && vi)
	{
		b->m_movement.x += mov.x;
	}

	if (dest.get_left() > r2.get_right() && vi)
	{
		b->m_movement.x -= mov.x;
	}

#if 0


	float itop = dest.get_bottom() - r2.get_top();
	float ibottom = r2.get_bottom() - dest.get_top();
	float ileft = dest.get_right() - r2.get_left();
	float iright = r2.get_right() - dest.get_left();

	float vert_penetration = std::min(itop, ibottom);
	float horiz_penetration = std::min(ileft, iright);
	if (vert_penetration > horiz_penetration) {
		if (itop > ibottom) {
			b->m_movement.y += mov.y;
			//mov.y = vert_penetration;
		} else {
			b->m_movement.y -= mov.y;
			//mov.y = -vert_penetration;
		}
	} else {
		if (ileft > iright) {
			b->m_movement.x += mov.x;
			//mov.x = horiz_penetration;
		} else {
			b->m_movement.x -= mov.y;
			//mov.x = -horiz_penetration;
		}
	}
#endif
}
