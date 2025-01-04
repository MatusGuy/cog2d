#include "collisionsystem.hpp"

#include "actormanager.hpp"
#include "logger.hpp"

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
	// TODO: There should be a way to specify if the interaction should
	// trigger if both are accepted or only one is.
	if (!m_groups[a->m_group][b->m_group] && !m_groups[b->m_group][a->m_group])
		return;

	Rect& r1 = a->m_bbox;
	Rect& r2 = b->m_bbox;
	Rect d1 = a->get_dest();
	Rect d2 = b->get_dest();
	Vector oldmov1 = a->m_movement;
	Vector oldmov2 = b->m_movement;

	// TODO: in_bounds function

	if (!d1.overlaps_exc(r2))
		return;

	float itop = d1.get_bottom() - d2.get_top();
	float ibottom = d2.get_bottom() - d1.get_top();
	float ileft = d1.get_right() - d2.get_left();
	float iright = d2.get_right() - d1.get_left();

	float vert_penetration = std::min(itop, ibottom);
	float horiz_penetration = std::min(ileft, iright);
	if (vert_penetration < horiz_penetration) {
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

	Vector avg = oldmov1.avg(oldmov2);
	a->m_movement += avg;
	b->m_movement += avg;
}
