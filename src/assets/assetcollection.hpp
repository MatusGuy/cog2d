#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <unordered_map>

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

using AssetId = std::uint32_t;

template<class>
class AssetCollection;

/**
 * @brief An AssetId that removes itself from the AssetCollection when destroyed.
 */
template<class Asset>
struct AssetKey
{
	~AssetKey();

	AssetId id;
	AssetCollection<Asset>* collection;

	operator AssetId() { return id; }
};

/*
template<class Asset>
using AssetKeyPtr = std::unique_ptr<AssetKey<Asset>>;
*/

template<class Asset>
class AssetCollection
{
public:
	AssetCollection();

	COG2D_FILE_TEMPLATE(AssetFile)
	AssetKey<Asset> load_file(const std::filesystem::path& path, std::string_view name = "",
	                          AssetId key = 0)
	{
		std::string assetname = name.empty() ? path.stem() : name;
		if (m_ids.find(assetname) != m_ids.end())
			return {m_ids[assetname], this};

		F file(path);
		AssetKey<Asset> asset = load(file, key);

		m_ids.insert({assetname, asset.id});

		return asset;
	}

	virtual AssetKey<Asset> load(IoDevice& device, AssetId key = 0) = 0;

	AssetKey<Asset> add(std::unique_ptr<Asset> asset, AssetId key = 0);
	void remove(AssetId key);

	Asset* get(AssetId key);

protected:
	std::unordered_map<AssetId, std::unique_ptr<Asset>> m_assets;
	std::unordered_map<std::string, AssetId> m_ids;
	AssetId m_nextkey;
};

class PixmapCollection : public AssetCollection<Texture>
{
public:
	AssetKey<Texture> load(IoDevice& device, AssetId key = 0) override;
};

COG2D_NAMESPACE_END_DECL

#endif  // ASSETMANAGER_HPP
