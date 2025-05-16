#pragma once

#include "cog2d/filesystem/file.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class AssetFile : public File
{
public:
	AssetFile(const std::filesystem::path& path);

	int open(OpenMode mode) override;
};

COG2D_NAMESPACE_END_DECL
