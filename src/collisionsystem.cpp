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
	Rect d1 = a->get_dest();
	Rect d2 = b->get_dest();
	Vector oldmov1 = a->m_movement;
	Vector oldmov2 = b->m_movement;

	// TODO: in_bounds function

	if (!r1.overlaps_exc(r2)/* || !d1.overlaps(r2) || !d2.overlaps(r1)*/)
		return;

#if 0
	float itop = r1.get_bottom() - r2.get_top();
	float ibottom = r2.get_bottom() - r1.get_top();
	float ileft = r1.get_right() - r2.get_left();
	float iright = r2.get_right() - r1.get_left();

	float vert_penetration = std::min(itop, ibottom);
	float horiz_penetration = std::min(ileft, iright);
	if (vert_penetration > horiz_penetration) {
		if (itop > ibottom) {
			a->m_movement.y += vert_penetration;
		} else {
			a->m_movement.y -= vert_penetration;
		}
	} else {
		if (ileft > iright) {
			a->m_movement.x += horiz_penetration;
		} else {
			a->m_movement.x -= horiz_penetration;
		}
	}
#endif
#if 0
	Vector diff = d1.pos - r2.pos;

	//if ((diff.x < -r1.size.x || diff.x > r2.size.x) && (diff.y < -r1.size.y || diff.y > r2.size.y))
	//	return;

	if (r1.get_bottom() > r2.get_top() && r2.get_bottom() > r1.get_top())
	{
		if (diff.x < 0) a->m_movement.x -= r1.size.x + diff.x;
		if (diff.x > 0) a->m_movement.x += r2.size.x - diff.x;
	}

	if (r1.get_right() > r2.get_left() && r2.get_right() > r1.get_left())
	{
		if (diff.y < 0) a->m_movement.y -= r1.size.y + diff.y;
		if (diff.y > 0) a->m_movement.y += r2.size.y - diff.y;
	}
#endif
#if 1
	Vector diff = d1.pos - r2.pos;
	bool hi = d1.get_right() > r2.get_left() && r2.get_right() > d1.get_left();
	bool vi = d1.get_bottom() > r2.get_top() && r2.get_bottom() > d1.get_top();
	//bool hi = d1.get_right() >= r2.get_right() && d1.get_left() <= r2.get_left();
	//bool vi = d1.get_bottom() >= r2.get_bottom() && d1.get_top() <= r2.get_top();

	if (d1.get_top() > r2.get_bottom() && hi)
	{
		a->m_movement.y += r2.size.y - diff.y;
		//if (b->get_destination().overlaps(r1))
		//	a->m_movement.y += oldmov.y;
	}

	if (d1.get_bottom() > r2.get_top() && hi)
	{
		a->m_movement.y -= r1.size.y + diff.y;
		//if (b->get_destination().overlaps(r1))
		//	a->m_movement.y -= oldmov.y;
	}

	if (d1.get_left() > r2.get_right() && vi)
	{
		a->m_movement.x += r2.size.x - diff.x;
		//if (b->get_destination().overlaps(r1))
		//	a->m_movement.x += oldmov.x;
	}

	if (d1.get_right() > r2.get_left() && vi)
	{
		a->m_movement.x -= r1.size.x + diff.x;
		//if (b->get_destination().overlaps(r1))
		//	a->m_movement.x -= oldmov.x;
	}
#endif

	Vector avg = oldmov1.avg(oldmov2);
	a->m_movement += avg;
	b->m_movement += avg;
}
