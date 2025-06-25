// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/screen.hpp"
#include "cog2d/scene/actormanager.hpp"
#include "cog2d/scene/viewport.hpp"

namespace cog2d {

class Scene;

/**
 * A stage... for the actors... you know... like a theater...
 * This class should control game state (paused, playing, game over, ...)
 */
class ActorStage : public Screen
{
public:
	ActorStage();

	void init() override;
	void update() override;
	void draw() override;
	bool event(SDL_Event* ev) override;

public:
	void set_current_scene(Scene* scene);
	inline Scene* get_current_scene() { return m_current_scene; }

private:
	Scene* m_current_scene;
	Viewport m_viewport;
};

}
