// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "cog2d/util/math/rect.hpp"
#include "cog2d/util/currenton.hpp"
#include "cog2d/tilemap/tilelayer.hpp"
#include "cog2d/ecs/builtins.hpp"
#include "cog2d/ecs/builtins/collision/collision.hpp"
#include "cog2d/ecs/world.hpp"

#ifndef COG2D_NUM_COLGROUPS
#error "COG2D_NUM_COLGROUPS undefined."
#endif

namespace cog2d {

struct EntityBase;
class TileLayer;

struct CompCollision
{
	Vector mov;
	std::uint32_t group;

	/// static and immovable
	bool stabile = false;
};

namespace ext {
void entity_collision(EntityId id, EntityBase** ent, CompCollision** col);
}

class CollisionSystem
{
public:
	enum Response : std::uint8_t
	{
		COLRESP_ACCEPT,
		COLRESP_REJECT
	};

public:
	bool m_groups[COG2D_NUM_COLGROUPS][COG2D_NUM_COLGROUPS];

	// TODO: multiple collision tilelayers?? Ehhhhhhhhhhh
	// FIXME: should this be owned by the tilemap??
	std::unique_ptr<TileLayer> m_tilelayer;

	std::vector<EntityId> m_entities;

public:
	CollisionSystem();

	void enable_interaction(std::uint32_t group_a, std::uint32_t group_b);

	void update();

	void rect_rect(EntityId a, EntityId b);

	//CollideInfo<Vector::type> rect_tile(EntityId ent, TileId tileid, const Rect& tilerect);
	//CollideInfo<Vector::type> rect_tilerect(Actor* a, const Rect& tilerect);

	Rect_t<int> get_tiles_overlapping(const Rect& rect);
};

}  //namespace cog2d
