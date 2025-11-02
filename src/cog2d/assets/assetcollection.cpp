// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "assetcollection.hpp"

#include <algorithm>
#include <functional>

#include <SDL2/SDL_image.h>

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

template<class A>
Asset<A> AssetCollection<A>::add(std::string_view name, A* asset)
{
	Asset<A> ptr(std::shared_ptr<A>(asset), this);
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

Asset<Texture> PixmapCollection::load(std::string_view name, File& device)
{
	Texture* texture = Texture::from_pixmap(std::move(device));

	if (!texture) {
		log::error("PixmapCollection",
		           fmt::format("Could not load pixmap \"{}\": {}", name, SDL_GetError()));
		return {};
	}

	return add(name, texture);
}

Asset<PixmapFont> PixmapFontCollection::load(std::string_view name, File& device)
{
	TomlResult data = toml_parse(device);
	PixmapFont* font = new PixmapFont;
	font->load(data.tbl);
	return add(name, font);
}

Asset<TileSet> TileSetCollection::load(std::string_view name, File& device)
{
	TomlResult data = toml_parse(device);
	TileSet* set = new TileSet;
	set->load(data.tbl);
	return add(name, set);
}

Asset<MusicTrack> MusicTrackCollection::load(std::string_view name, File& device)
{
	TomlResult data = toml_parse(device);
	MusicTrack* track = new MusicTrack;
	track->load(data.tbl);
	return add(name, track);
}

Asset<SoundEffect> SoundEffectCollection::load(std::string_view name, File& device)
{
	SoundEffect* sound = new SoundEffect;
	sound->load(device);
	audio::add_sound(*sound);
	//AudioEngine::get().add_source(sound);
	return add(name, sound);
}

}  //namespace cog2d
