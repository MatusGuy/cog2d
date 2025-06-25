// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "cog2d/filesystem/assetfile.hpp"
#include "cog2d/video/texture.hpp"
#include "cog2d/video/font/pixmapfont.hpp"
#include "cog2d/scene/tilemap/tileset.hpp"

#include "cog2d/assets/asset.hpp"
#include "cog2d/filesystem/document/tomldocument.hpp"

COG2D_NAMESPACE_BEGIN_DECL

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

	COG2D_FILE_TEMPLATE(AssetFile)
	Asset<A> load_file(const std::filesystem::path& path, std::string_view name = "")
	{
		std::string assetname = name.empty() ? path.stem().string() : std::string{name};
		auto it = m_assets.find(assetname);
		if (it != m_assets.end())
			return Asset<A>((*it).second.lock(), this);

		F file(path);
		return load(assetname, file);
	}

	virtual Asset<A> load(std::string_view name, IoDevice& device) = 0;

	Asset<A> add(std::string_view name, std::unique_ptr<A> asset);
	void try_remove_key(std::string_view name, bool check = true);
	void try_remove_asset(AssetRef<A> asset);

protected:
	std::unordered_map<std::string, AssetRef<A>> m_assets;

private:
	static void reap_cache_entry();
};

class PixmapCollection : public AssetCollection<Texture>
{
public:
	Asset<Texture> load(std::string_view name, IoDevice& device) override;
};

class PixmapFontCollection : public AssetCollection<PixmapFont>
{
public:
	Asset<PixmapFont> load(std::string_view name, IoDevice& device) override;
};

class TileSetCollection : public AssetCollection<TileSet>
{
public:
	Asset<TileSet> load(std::string_view name, IoDevice& device) override;
	Asset<TileSet> load(std::string_view name, TomlDocument& data);
};

COG2D_NAMESPACE_END_DECL
