// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "audioengine.hpp"

#include "cog2d/util/logger.hpp"

namespace cog2d {

void AudioEngine::add_source(std::unique_ptr<MixerSource> source)
{
	m_sources.push_back(std::move(source));
}

}  //namespace cog2d
