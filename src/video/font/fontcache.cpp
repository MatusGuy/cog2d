// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "fontcache.hpp"

#include "cog2d/program.hpp"
#include "cog2d/video/font/font.hpp"

namespace cog2d {

FontCache::FontCache(Font& font)
    : m_font(font)
{
}

Texture* FontCache::get(const std::string& text)
{
	COG2D_USE_PROGRAM;

	auto it = m_strings.find(text);
	if (it != m_strings.end()) {
		CacheEntry& entry = it->second;
		entry.last_use = program.m_prog_time;
		return entry.texture.get();
	} else {
		std::unique_ptr<Texture> texture = m_font.create_text(text);
		Texture* ptr = texture.get();
		m_strings.insert(std::make_pair<std::string, CacheEntry>(std::string(text),
		                                                         CacheEntry{std::move(texture),
		                                                                    program.m_prog_time}));
		return ptr;
	}
}

bool FontCache::update_gc()
{
	COG2D_USE_PROGRAM;

	static std::int32_t last_cleanup = 0;
	if (last_cleanup == 0)
		last_cleanup = program.m_prog_time;

	// Make GC more aggressive when there are more strings (can happen with constant string updates)
	std::int32_t GC_TIME = (1 + 15.0 / (int) (((m_strings.size()) / 32.f) + 1)) * 1000.f;
	std::int32_t GC_INTERVAL = GC_TIME;
	bool did_cleanup = false;
	std::int32_t curr = program.m_prog_time;

	if (last_cleanup >= curr - GC_INTERVAL) {
		return false;
	}
	//Logger::debug("Trying Font cleanup...");
	last_cleanup = curr;

	auto it = m_strings.begin();
	while (it != m_strings.end()) {
		CacheEntry& entry = it->second;
		if (entry.last_use < curr - GC_TIME) {
			//Logger::debug("Cleaned up \"" + it->first + "\"");
			auto oldit = std::next(it, 1);
			m_strings.erase(it);
			did_cleanup = true;
			it = oldit;
			continue;
		}
		++it;
	}

	return did_cleanup;
}

}  //namespace cog2d
