#include "tilescene.hpp"

namespace cog2d {

TileScene::TileScene()
{

}


void TileScene::init()
{
	prepare_context();
}

void TileScene::update()
{
	Scene::update();

	//prepare_context();

	//for (auto it = m_layers.begin(); it != m_layers.end(); ++it) {
	//	auto layer = *it;
	//	//layer.
	//}
}

void TileScene::draw()
{
	prepare_context();
	m_map.draw();
	Scene::draw();
}

}
