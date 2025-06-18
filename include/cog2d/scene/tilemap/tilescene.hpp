// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>

#include "cog2d/scene/scene.hpp"
#include "cog2d/scene/tilemap/tilemap.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class TileScene : public Scene
{
public:
	TileMap m_map;

public:
	TileScene();

	void init() override;
	void update() override;
	void draw() override;
};

COG2D_NAMESPACE_END_DECL
