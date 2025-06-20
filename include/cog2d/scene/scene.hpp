// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/scene/actormanager.hpp"
#include "cog2d/scene/collision/collisionsystem.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Viewport;
class ActorStage;

class Scene
{
public:
	ActorStage* m_stage;

public:
	Scene();

	virtual void init() = 0;

	virtual void update();
	virtual void draw();

protected:
	void prepare_context();

protected:
	ActorManager m_actormanager;
	CollisionSystem m_collisionsystem;
};

COG2D_NAMESPACE_END_DECL
