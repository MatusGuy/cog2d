#include "collisionsystem.hpp"

#include "actormanager.hpp"
#include "logger.hpp"

CollisionSystem::CollisionSystem()
{

}

void CollisionSystem::update(ActorManager &actormanager)
{
    ActorManager::Actors &actors = actormanager.get_actors();

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


	CollisionBody* target = a->m_static ? b : a;
	CollisionBody* other = target == a ? b : a;

	Rect& r1 = target->m_bbox;
	Rect& r2 = other->m_bbox;
	Rect d1 = target->get_dest();
	Rect d2 = other->get_dest();
	Vector oldmov1 = target->m_mov;
	Vector oldmov2 = other->m_mov;

	if (!d1.overlaps(d2))
		return;

	CollisionSystem::Response resp1 = a->collision(b);
	CollisionSystem::Response resp2 = b->collision(a);
	if (resp1 == COLRESP_REJECT || resp2 == COLRESP_REJECT)
		return;

	float itop = d1.get_bottom() - d2.get_top();
	float ibottom = d2.get_bottom() - d1.get_top();
	float ileft = d1.get_right() - d2.get_left();
	float iright = d2.get_right() - d1.get_left();

	float vert_penetration = std::min(itop, ibottom);
	float horiz_penetration = std::min(ileft, iright);
	if (vert_penetration < horiz_penetration) {
		if (itop > ibottom) {
			target->m_mov.y += vert_penetration;
		} else {
			target->m_mov.y -= vert_penetration;
		}
	} else {
		if (ileft > iright) {
			target->m_mov.x += horiz_penetration;
		} else {
			target->m_mov.x -= horiz_penetration;
		}
	}


	if (!a->m_static && !b->m_static)
	{
		Vector avg = oldmov1.avg(oldmov2);
		//if (!a->m_static)
			a->m_mov += avg;

		//if (!b->m_static)
			b->m_mov += avg;
	}
}
