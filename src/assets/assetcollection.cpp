// SPDX-License-Identifier: LGPL-3.0-only

#include "assetcollection.hpp"

#include <algorithm>
#include <functional>

#include <SDL2/SDL_image.h>

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

template<class A>
Asset<A> AssetCollection<A>::add(std::string_view name, std::unique_ptr<A> asset)
{
	Asset<A> ptr(std::shared_ptr<A>(asset.release()), this);
	m_assets.insert({std::string(name), AssetRef<A>(ptr)});
	return ptr;
}

template<class A>
void AssetCollection<A>::try_remove_asset(AssetRef<A> asset)
{
	std::shared_ptr<A> ptr = asset.lock();
	auto it = std::find_if(m_assets.begin(), m_assets.end(),
	                       [&ptr](auto&& p) { return p.second.lock() == ptr; });
	ptr.reset();

	if (it != m_assets.end())
		try_remove_key((*it).first, false);
}

template<class A>
void AssetCollection<A>::try_remove_key(std::string_view name, bool check)
{
	std::string key(name);

	if (check) {
		auto it = m_assets.find(key);
		if (it == m_assets.end())
			return;
	}

	const AssetRef<A> ref = m_assets.at(key);
	if (!ref.expired())
		return;

	m_assets.erase(key);
}

template<class T>
Asset<T>::~Asset()
{
	if (collection)
		collection->try_remove_asset(*this);

	this->reset();
}

Asset<Texture> PixmapCollection::load(std::string_view name, IoDevice& device)
{
	COG2D_USE_GRAPHICSENGINE;

	if (!device.is_open())
		device.open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	SDL_Texture* texture = IMG_LoadTexture_RW(graphicsengine.get_renderer(), device.to_sdl(), 1);

	if (!texture) {
		COG2D_LOG_ERROR("PixmapCollection",
		                fmt::format("Could not load pixmap \"{}\": {}", name, SDL_GetError()));
		return {};
	}

	return add(name, std::make_unique<Texture>(texture));
}

Asset<PixmapFont> PixmapFontCollection::load(std::string_view name, IoDevice& device)
{
	auto font = new PixmapFont();
	font->load(std::move(device));
	return add(name, std::unique_ptr<AssetType>(std::move(font)));
}

Asset<TileSet> TileSetCollection::load(std::string_view name, IoDevice& device)
{
	if (!device.is_open())
		device.open(IoDevice::OPENMODE_READ);

	toml::table data = toml::parse(device);

	device.close();

	return load(name, data);
}

Asset<TileSet> TileSetCollection::load(std::string_view name, toml::table& data)
{
	std::unique_ptr<TileSet> set = std::make_unique<TileSet>();
	set->load(data);
	return add(name, std::move(set));
}

COG2D_NAMESPACE_END_IMPL
