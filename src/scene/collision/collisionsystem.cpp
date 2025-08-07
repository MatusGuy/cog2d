// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "collisionsystem.hpp"

#include "cog2d/scene/actor.hpp"
#include "cog2d/scene/actorrefsiterator.hpp"
#include "cog2d/util/logger.hpp"
#include "cog2d/util/timing.hpp"
#include "cog2d/scene/tilemap/tilemap.hpp"
#include "cog2d/scene/collision/collision.hpp"

namespace cog2d {

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::update()
{
	ActorRefsIterator it(m_actors.begin(), m_actors);
	if (m_tilelayer) {
		while (it.m_it != m_actors.end()) {
			Actor* a = *it;

			Rect_t<int> tiles = m_tilelayer->get_tiles_overlapping(a->get_dest());
			Vector_t<int> tilepos = tiles.pos;
			const Vector_t<std::uint16_t> tilesz = m_tilelayer->m_map->m_tile_sz;
			//COG2D_LOG_DEBUG(fmt::format("{}", tiles));
			for (; tilepos.x < tiles.get_right(); ++tilepos.x) {
				for (; tilepos.y < tiles.get_bottom(); ++tilepos.y) {
					Rect tilerect(tilepos * static_cast<Vector>(tilesz),
					              static_cast<Vector>(tilesz));

					//COG2D_LOG_DEBUG(fmt::format("{}", a->classidx()));
					//COG2D_LOG_DEBUG(fmt::format("{}, {}", tilepos,
					//                            m_tilelayer->get_tile_id(tilepos)));
					//COG2D_LOG_DEBUG(fmt::format("{}, {}", tilepos, tilerect));

					rect_tile(a, m_tilelayer->get_tile_id(tilepos), tilerect);
				}
				tilepos.y = tiles.pos.y;
			}

			++it;
		}
	}

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

	CollideInfo<Vector::type> info = Collision::rect_rect<Vector::type>(d1, d2);

	info.apply(mov);

	if (!col_a.heavy && !col_b.heavy) {
		Vector avg = oldmov1.avg(oldmov2);
		// if (!a->m_static)
		//a->m_mov += avg;

		// if (!b->m_static)
		if (info.left || info.right) {
			col_a.mov.x += avg.x;
			col_b.mov.x += avg.x;
		} else {
			col_a.mov.y += avg.y;
			col_b.mov.y += avg.y;
		}
	}
}

void CollisionSystem::rect_tile(Actor* a, TileId tileid, const Rect& tilerect)
{
	switch (tileid) {
	case 0:
		break;

	case 1:
	default:
		rect_tilerect(a, tilerect);
		break;
	}
}

void CollisionSystem::rect_tilerect(Actor* a, const Rect& tilerect)
{
	Rect d1 = a->get_dest();
	Rect d2 = tilerect;

	if (!d1.overlaps(d2))
		return;

	/*
	CollisionSystem::Response resp1 = a->collision(b);
	CollisionSystem::Response resp2 = b->collision(a);
	if (resp1 == COLRESP_REJECT || resp2 == COLRESP_REJECT)
		return;
	*/

	Vector& mov = a->col().mov;

	CollideInfo<Vector::type> info = Collision::rect_rect<Vector::type>(d1, tilerect);
	info.apply(mov);
}

}  //namespace cog2d
