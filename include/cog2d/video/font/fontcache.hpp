// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <string_view>
#include <unordered_map>
#include <memory>

#include "cog2d/util/timing.hpp"
#include "cog2d/video/texture.hpp"

namespace cog2d {

class Font;

class FontCache
{
public:
	struct CacheEntry
	{
		std::unique_ptr<Texture> texture;
		TimePoint last_use;
	};

public:
	Font& m_font;
	std::unordered_map<std::string, CacheEntry> m_strings;

public:
	FontCache(Font& font);

	Texture* get(const std::string& text);
	bool update_gc();
};

}  //namespace cog2d
