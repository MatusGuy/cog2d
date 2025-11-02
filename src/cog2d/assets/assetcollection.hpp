// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "cog2d/filesystem/file.hpp"
#include "cog2d/video/texture.hpp"
#include "cog2d/video/font/pixmapfont.hpp"
#include "cog2d/tilemap/tileset.hpp"
#include "cog2d/audio/musictrack.hpp"
#include "cog2d/audio/soundeffect.hpp"

#include "cog2d/assets/asset.hpp"
#include "cog2d/util/toml.hpp"

namespace cog2d {

template<class A>
using AssetRefs = std::unordered_map<std::string, AssetRef<A>>;

template<class A>
class AssetCollection
{
public:
	using AssetType = A;

public:
	AssetCollection()
	    : m_assets()
	{
	}

	Asset<A> load_file(std::string_view path)
	{
		auto it = m_assets.find(path.data());
		if (it != m_assets.end())
			return Asset<A>((*it).second.lock(), this);

		File file = File::from_asset(path);
		return load(path, file);
	}

	// FIXME: virtual?? really??
	virtual Asset<A> load(std::string_view name, File& device) = 0;

	Asset<A> add(std::string_view name, A* asset);
	void try_remove_key(std::string_view name, bool check = true);
	void try_remove_asset(AssetRef<A> asset);

	inline const AssetRefs<A>& get_assets() const { return m_assets; }

protected:
	AssetRefs<A> m_assets;

private:
	static void reap_cache_entry();
};

class PixmapCollection : public AssetCollection<Texture>
{
public:
	Asset<Texture> load(std::string_view name, File& device) override;
};

class PixmapFontCollection : public AssetCollection<PixmapFont>
{
public:
	Asset<PixmapFont> load(std::string_view name, File& device) override;
};

class TileSetCollection : public AssetCollection<TileSet>
{
public:
	Asset<TileSet> load(std::string_view name, File& device) override;
};

class MusicTrackCollection : public AssetCollection<MusicTrack>
{
public:
	Asset<MusicTrack> load(std::string_view name, File& device) override;
};

class SoundEffectCollection : public AssetCollection<SoundEffect>
{
public:
	Asset<SoundEffect> load(std::string_view name, File& device) override;
};

}  //namespace cog2d
