// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/util/parsing.hpp"

namespace cog2d {

class TileMap;
class TileLayer;
class ActorManager;
class Actor;

class BinTileMapParser : public Parser<TileMap>
{
	ActorManager& m_actormanager;

public:
	BinTileMapParser(ActorManager& actormanager)
		: m_actormanager(actormanager)
	{
	}

	void parse(IoDevice& device, TileMap& result) override;

private:
	void parse_object_group(IoDevice& device);
	bool parse_property(IoDevice& device, int idx, Actor& actor);
};

class BinTileLayerParser : public Parser<TileLayer>
{
public:
	BinTileLayerParser() {}

	void parse(IoDevice& device, TileLayer& result) override;
};

}  //namespace cog2d
