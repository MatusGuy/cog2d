#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "cog2d/filesystem/assetfile.hpp"
#include "cog2d/video/texture.hpp"

COG2D_NAMESPACE_BEGIN_DECL

enum AssetType
{
	ASSET_IMAGE,
	ASSET_SFX,
	ASSET_MUSIC,
	ASSET_FONT,

	ASSET_COUNT
};

template<class A>
class AssetCollection;

template<class T>
using AssetRef = std::weak_ptr<T>;

template<class T>
class Asset : public std::shared_ptr<T>
{
public:
	AssetCollection<T>* collection = nullptr;

public:
	Asset() {}
	Asset(std::shared_ptr<T> ptr, AssetCollection<T>* col);
	~Asset();

	inline bool valid() { return collection != nullptr && this->get() != nullptr; }
};

template<class A>
class AssetCollection
{
public:
	AssetCollection()
	    : m_assets()
	{
	}

	COG2D_FILE_TEMPLATE(AssetFile)
	Asset<A> load_file(const std::filesystem::path& path, std::string_view name = "")
	{
		std::string assetname = name.empty() ? path.stem() : name;
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

COG2D_NAMESPACE_END_DECL
