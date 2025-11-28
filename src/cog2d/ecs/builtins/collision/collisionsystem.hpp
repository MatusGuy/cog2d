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

enum CollisionResponse : std::uint8_t
{
	COLRESP_ACCEPT,
	COLRESP_REJECT
};

struct CompCollision
{
	Vector mov;
	std::uint32_t group;

	/// static and immovable
	bool stabile = false;
};

namespace ext {
void entity_get_collision(EntityId id, EntityBase** ent, CompCollision** col);
CollisionResponse entity_collision(EntityBase& a, EntityBase& b);
CollisionResponse entity_collision_tile(EntityBase& base, std::size_t tileidx);
}  //namespace ext

class CollisionSystem
{
public:
	bool m_groups[COG2D_NUM_COLGROUPS][COG2D_NUM_COLGROUPS] = {0};
	bool m_tile_colgroups[COG2D_NUM_COLGROUPS] = {0};  // Collision groups that interact with tiles

	std::vector<EntityId> m_entities;

	TileMap* m_tilemap = nullptr;

public:
	CollisionSystem();

	void enable_interaction(std::uint32_t group_a, std::uint32_t group_b, bool value = true);
	void enable_tile_interaction(std::uint32_t group, bool value = true);

	void update();

	void rect_rect(EntityId a, EntityId b);

	CollideInfo<Vector::type> rect_tile(EntityId id, std::size_t tileidx);
	CollideInfo<Vector::type> rect_tilerect(EntityId id, std::size_t tileidx);

	Rect_t<int> get_tiles_overlapping(const Rect& rect);
};

}  //namespace cog2d
