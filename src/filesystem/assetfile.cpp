// SPDX-License-Identifier: LGPL-3.0-only

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
	auto pathit = m_path.begin();

	for (auto it = assetpath.begin(); it != assetpath.end(); ++it) {
		int i = std::distance(assetpath.begin(), it);

		std::advance(pathit, i);
		if (*it != *pathit)
			// This asset is outside of the asset path.
			return -1;

		pathit = m_path.begin();
	}

	return File::open(mode);
}

COG2D_NAMESPACE_END_IMPL
