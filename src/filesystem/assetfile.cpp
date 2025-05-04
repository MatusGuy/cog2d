#include "assetfile.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

AssetFile::AssetFile(const std::filesystem::path& path)
    : File(path)
{

}

int AssetFile::open(OpenMode mode)
{
	std::filesystem::path assetpath = COG2D_ASSET_PATH;

	m_path = std::filesystem::absolute(assetpath / m_path);

	for (auto it = assetpath.begin(); it < assetpath.end(); ++it) {
		int i = std::distance(assetpath.begin(), it);
		if (*it != *(m_path.begin() + i))
			// This asset is outside of the asset path.
			return -1;
	}

	return File::open(mode);
}

COG2D_NAMESPACE_END_IMPL
