// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "collisionsystem.hpp"

#include "cog2d/scene/actor.hpp"
#include "cog2d/scene/actorrefsiterator.hpp"
#include "cog2d/util/logger.hpp"
#include "cog2d/util/timing.hpp"

namespace cog2d {

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::update()
{
	ActorRefsIterator it_a(m_actors.begin(), m_actors);
	while (it_a.m_it != m_actors.end()) {
		Actor* a = *it_a;

		ActorRefsIterator it_b(std::next(m_actors.begin(), 1), m_actors);
		while (it_b.m_it != m_actors.end()) {
			Actor* b = *it_b;

			if (a == b) {
				++it_b;
				continue;
			}

			rect_rect(a, b);

			++it_b;
		}

		++it_a;
	}

	for (Actor* actor : m_actors) {
		actor->apply_movement();
	}
}

void CollisionSystem::rect_rect(Actor* a, Actor* b)
{
	ActorComps::Collision& col_a = a->col();
	ActorComps::Collision& col_b = b->col();

	// TODO: There should be a way to specify if the interaction should
	// trigger if both are accepted or only one is.
	if (!m_groups[col_a.group][col_b.group] && !m_groups[col_a.group][col_b.group])
		return;

	Actor* target = col_a.heavy ? b : a;
	Actor* other = target == a ? b : a;

	Rect& r1 = target->bbox();
	Rect& r2 = other->bbox();
	Rect d1 = target->get_dest();
	Rect d2 = other->get_dest();
	Vector oldmov1 = target->col().mov;
	Vector oldmov2 = other->col().mov;

	if (!d1.overlaps_exc(d2))
		return;

	CollisionSystem::Response resp1 = a->collision(b);
	CollisionSystem::Response resp2 = b->collision(a);
	if (resp1 == COLRESP_REJECT || resp2 == COLRESP_REJECT)
		return;

	Vector& mov = target->col().mov;

	float itop = d1.get_bottom() - d2.get_top();
	float ibottom = d2.get_bottom() - d1.get_top();
	float ileft = d1.get_right() - d2.get_left();
	float iright = d2.get_right() - d1.get_left();

	bool move_horiz;
	float vert_penetration = std::min(itop, ibottom);
	float horiz_penetration = std::min(ileft, iright);
	if (vert_penetration < horiz_penetration) {
		if (itop > ibottom) {
			mov.y += vert_penetration;
		} else {
			mov.y -= vert_penetration;
		}
		move_horiz = false;
	} else {
		if (ileft > iright) {
			mov.x += horiz_penetration;
		} else {
			mov.x -= horiz_penetration;
		}
		move_horiz = true;
	}

	if (!col_a.heavy && !col_b.heavy) {
		Vector avg = oldmov1.avg(oldmov2);
		// if (!a->m_static)
		//a->m_mov += avg;

		// if (!b->m_static)
		if (move_horiz) {
			col_a.mov.x += avg.x;
			col_b.mov.x += avg.x;
		} else {
			col_a.mov.y += avg.y;
			col_b.mov.y += avg.y;
		}
	}
}

}  //namespace cog2d
