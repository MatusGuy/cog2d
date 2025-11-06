// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "collisionsystem.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/util/timing.hpp"

namespace cog2d {

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::update()
{
	/*
	ActorRefsIterator it(m_actors.begin(), m_actors);
	if (m_tilelayer && true) {
		while (it.m_it != m_actors.end()) {
			Actor* a = *it;

			//Vector mov = a->col().mov;
			Rect_t<int> tiles = m_tilelayer->get_tiles_overlapping(a->get_dest());
			Vector_t<int> tilepos = tiles.pos;
			const Vector_t<std::uint16_t> tilesz = m_tilelayer->m_map->m_tile_sz;
			CollideInfo<Vector::type> collideinfo;
			//log::debug(fmt::format("{}", tiles));
			for (; tilepos.x < tiles.get_right(); ++tilepos.x) {
				for (; tilepos.y < tiles.get_bottom(); ++tilepos.y) {
					Rect tilerect(tilepos * static_cast<Vector>(tilesz),
								  static_cast<Vector>(tilesz));

					//log::debug(fmt::format("{}", a->classidx()));
					//log::debug(fmt::format("{}, {}", tilepos,
					//                            m_tilelayer->get_tile_id(tilepos)));
					//log::debug(fmt::format("{}, {}", tilepos, tilerect));

					CollideInfo<Vector::type>
						collideinfo2 = rect_tile(a, m_tilelayer->get_tile_id(tilepos), tilerect);
					//collideinfo.merge(collideinfo2);
					//collideinfo2.apply(mov);
					collideinfo2.apply(a->col().mov);
				}
				tilepos.y = tiles.pos.y;
			}
			//collideinfo.apply(a->col().mov);
			//a->col().mov = mov;

			++it;
		}
	}
	*/

	for (int i = 0; i < m_entities.size(); ++i) {
		for (int j = 1; j < m_entities.size(); ++j) {
			if (i == j)
				continue;

			rect_rect(m_entities[i], m_entities[j]);
		}
	}

	for (int i = 0; i < m_entities.size(); ++i) {
		/// Apply movements
		EntityBase* ent;
		CompCollision* col;
		ext::entity_collision(m_entities[i], &ent, &col);
		ent->bbox.pos += col->mov;
		log::debug(fmt::format("{} {}", col->mov, ent->bbox));
		col->mov = {0, 0};
	}
}

void CollisionSystem::rect_rect(EntityId a, EntityId b)
{
	EntityBase* ent_a;
	EntityBase* ent_b;
	CompCollision* col_a;
	CompCollision* col_b;

	ext::entity_collision(a, &ent_a, &col_a);
	ext::entity_collision(b, &ent_b, &col_b);

	if (col_a->mov.is_null() && col_b->mov.is_null())
		return;

	// TODO: There should be a way to specify if the interaction should
	// trigger if both are accepted or only one is.
	//if (!m_groups[col_a->group][col_b->group] && !m_groups[col_a->group][col_b->group])
	//	return;

	Rect dest_a = ent_a->bbox.moved(col_a->mov);
	Rect dest_b = ent_b->bbox.moved(col_b->mov);
	Vector oldmov_a = col_a->mov;
	Vector oldmov_b = col_b->mov;

	if (!dest_a.overlaps_exc(dest_b))
		return;

	//CollisionSystem::Response resp1 = a->collision(b);
	//CollisionSystem::Response resp2 = b->collision(a);
	//if (resp1 == COLRESP_REJECT || resp2 == COLRESP_REJECT)
	//	return;

	Vector& mov = col_b->mov;

	CollideInfo<Vector::type> info = collision::rect_rect<Vector::type>(dest_a, dest_b);

	info.apply(mov);

	if (!col_a->stabile && !col_b->stabile) {
		Vector avg = oldmov_a.avg(oldmov_b);
		// if (!a->m_static)
		//a->m_mov += avg;

		// if (!b->m_static)
		if (info.left || info.right) {
			col_a->mov.x += avg.x;
			col_b->mov.x += avg.x;
		} else {
			col_a->mov.y += avg.y;
			col_b->mov.y += avg.y;
		}
	}
}
/*
CollideInfo<Vector::type> CollisionSystem::rect_tile(Actor* a, TileId tileid, const Rect& tilerect)
{
	switch (tileid) {
	case 0:
		return {};

	case 1:
	default:
		return rect_tilerect(a, tilerect);
	}
}

CollideInfo<Vector::type> CollisionSystem::rect_tilerect(Actor* a, const Rect& tilerect)
{
	Rect d1 = a->get_dest();
	Rect d2 = tilerect;

	if (!d1.overlaps(d2))
		return {};

	//CollisionSystem::Response resp1 = a->collision(b);
	//CollisionSystem::Response resp2 = b->collision(a);
	//if (resp1 == COLRESP_REJECT || resp2 == COLRESP_REJECT)
	//	return;

	Vector& mov = a->col().mov;

	CollideInfo<Vector::type> info = collision::rect_rect<Vector::type>(d1, tilerect);
	return info;
}
*/
}  //namespace cog2d
