// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>
#include <cstdint>

#include "cog2d/util/math/rect.hpp"
#include "cog2d/util/currenton.hpp"
#include "cog2d/scene/actorcontainers.hpp"
#include "cog2d/scene/tilemap/tilelayer.hpp"
#include "cog2d/scene/collision/collision.hpp"

namespace cog2d {

class CollisionBody;
class ActorManager;
class TileLayer;

class CollisionSystem
{
public:
	/// @brief A collision group.
	// std::vector has a specializaion for bool that
	// tries to use bits for individual elements, so
	// no need to worry about space here.
	// FIXME: does this really need to be dynamic?
	using Group = std::vector<bool>;

	enum Response : std::uint8_t
	{
		COLRESP_ACCEPT,
		COLRESP_REJECT
	};

public:
	ActorManager* m_manager;
	std::vector<Group> m_groups;
	ActorRefs m_actors;

	// TODO: multiple collision tilelayers?? Ehhhhhhhhhhh
	// FIXME: should this be owned by the tilemap??
	std::unique_ptr<TileLayer> m_tilelayer;

public:
	CollisionSystem();

	void update();

	void rect_rect(Actor* a, Actor* b);

	CollideInfo<Vector::type> rect_tile(Actor* a, TileId tileid, const Rect& tilerect);
	CollideInfo<Vector::type> rect_tilerect(Actor* a, const Rect& tilerect);

	Rect_t<int> get_tiles_overlapping(const Rect& rect);
};

}  //namespace cog2d
