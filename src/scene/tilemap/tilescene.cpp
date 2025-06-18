#include "tilescene.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TileScene::TileScene()
{

}


void TileScene::init()
{
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

COG2D_NAMESPACE_END_IMPL
