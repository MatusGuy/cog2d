// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>

namespace cog2d {

/*
enum AssetType
{
    ASSET_IMAGE,
    ASSET_SFX,
    ASSET_MUSIC,
    ASSET_FONT,

    ASSET_COUNT
};
*/

template<class A>
class AssetCollection;

template<class T>
using AssetRef = std::weak_ptr<T>;

template<class T>
class Asset;

template<class T>
class Asset : public std::shared_ptr<T>
{
public:
	AssetCollection<T>* collection = nullptr;

public:
	Asset() {}
	Asset(std::shared_ptr<T> ptr, AssetCollection<T>* col)
	    : std::shared_ptr<T>(std::move(ptr)),
	      collection(col)
	{
	}

	// See assetcollection.cpp for definition.
	~Asset();

	inline bool valid() { return collection != nullptr && this->get() != nullptr; }
};

}  //namespace cog2d
