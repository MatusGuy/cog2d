#include "scene.hpp"


Scene::Scene():
	m_actormanager(),
	m_collisionsystem()
{

}

void Scene::init()
{
	ActorManager::s_current = &m_actormanager;
	CollisionSystem::s_current = &m_collisionsystem;
}


void Scene::update()
{

}

void Scene::draw()
{
}
