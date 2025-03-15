#ifndef SCENE_HPP
#define SCENE_HPP

#include "screen.hpp"
#include "actormanager.hpp"

class Scene : public Screen
{
public:
	Scene();

	void init() override;
	void update() override;
	void draw() override;
	bool event(SDL_Event *ev) override;

protected:
	ActorManager m_actormanager;
	CollisionSystem m_collisionsystem;
};

#endif // SCENE_HPP
