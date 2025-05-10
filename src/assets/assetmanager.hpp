#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <filesystem>
#include <string>
#include <unordered_map>
#include <list>

#include "cog2d/assets/assetcollection.hpp"

COG2D_NAMESPACE_BEGIN_DECL

#define COG2D_USE_ASSETMANAGER COG2D_USING(AssetManager, assetmanager)
class AssetManager : public Currenton<AssetManager>
{
public:
	PixmapCollection pixmaps;

public:
	AssetManager();
};

COG2D_NAMESPACE_END_DECL

#endif  // ASSETMANAGER_H
