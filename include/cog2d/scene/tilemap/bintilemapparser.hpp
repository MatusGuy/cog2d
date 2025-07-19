// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/util/parsing.hpp"

namespace cog2d {

class TileMap;
class TileLayer;

class BinTileMapParser : public Parser<TileMap>
{
public:
	BinTileMapParser();

	void parse(IoDevice& device, TileMap& result) override;
};

class BinTileLayerParser : public Parser<TileLayer>
{
public:
	BinTileLayerParser() {}

	void parse(IoDevice& device, TileLayer& result) override;
};

}  //namespace cog2d
