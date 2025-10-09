// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "fontcache.hpp"

#include "cog2d/program.hpp"
#include "cog2d/video/font/ttffont.hpp"

namespace cog2d {

FontCache::FontCache(TtfFont& font)
    : m_font(font)
{
}

Texture* FontCache::get(const std::string& text)
{
	auto it = m_strings.find(text);
	if (it != m_strings.end()) {
		CacheEntry& entry = it->second;
		entry.last_use = s_program.prog_time;
		return entry.texture.get();
	} else {
		Texture* texture = m_font.create_text(text);
		m_strings.insert(std::make_pair<std::string, CacheEntry>(std::string(text),
		                                                         CacheEntry{std::move(texture),
		                                                                    s_program.prog_time}));
		return texture;
	}
}

bool FontCache::update_gc()
{
	static TimePoint last_cleanup;
	if (last_cleanup == TimePoint(Duration::zero()))
		last_cleanup = s_program.prog_time;

	// Make GC more aggressive when there are more strings (can happen with constant string updates)
	Duration GC_TIME((1 + 15 / static_cast<int>(((m_strings.size()) / 32.f) + 1)) * 1000);
	Duration GC_INTERVAL = GC_TIME;
	bool did_cleanup = false;
	TimePoint curr = s_program.prog_time;

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
