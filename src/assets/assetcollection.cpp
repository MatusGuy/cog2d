#include "assetcollection.hpp"

#include <SDL2/SDL_image.h>

#include "cog2d/video/graphicsengine.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

template<class Asset>
AssetKey<Asset>::~AssetKey()
{
	collection->remove(id);
}

template<class Asset>
AssetCollection<Asset>::AssetCollection()
    : m_assets(),
      m_nextkey()
{
}

template<class Asset>
AssetKey<Asset> AssetCollection<Asset>::add(std::unique_ptr<Asset> asset, AssetId key)
{
	AssetId newkey = key == 0 ? m_nextkey : key;

	if (key == m_nextkey && m_nextkey != 0) {
		// Uhhhh... No! Don't do that! Try this key instead.
		do {
			key++;
		} while (m_assets.find(key) != m_assets.end());
	}

	m_assets.insert({newkey, std::move(asset)});

	if (key == 0) {
		do {
			m_nextkey++;
		} while (m_assets.find(m_nextkey) != m_assets.end());
	}

	return {newkey, this};
}

template<class Asset>
void AssetCollection<Asset>::remove(AssetId key)
{
	m_assets.erase(key);
	m_nextkey = key;
}

template<class Asset>
Asset* AssetCollection<Asset>::get(AssetId key)
{
	auto it = m_assets.find(key);
	if (it == m_assets.end())
		return nullptr;

	return (*it)->get();
}

AssetKey<Texture> PixmapCollection::load(IoDevice& device, AssetId key)
{
	COG2D_USE_GRAPHICSENGINE;

	if (!device.is_open())
		device.open(IoDevice::OPENMODE_READ);

	SDL_Texture* texture = IMG_LoadTexture_RW(graphicsengine.get_renderer(), device.to_sdl(), 1);
	return add(std::make_unique<Texture>(texture), key);
}

COG2D_NAMESPACE_END_IMPL
