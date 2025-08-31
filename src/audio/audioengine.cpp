// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "audioengine.hpp"

#include "cog2d/util/logger.hpp"

namespace cog2d {

void AudioEngine::add_source(MixerSource* source)
{
	if (source->m_engine != nullptr)
		source->m_engine->remove_source(source);

	source->m_engine = this;
	source->m_id = m_sources.size();
	m_sources.push_back(source);
}

void AudioEngine::remove_source(MixerSource* source)
{
	if (source->m_engine != this)
		return;

	source->m_engine = nullptr;
	m_sources.erase(m_sources.begin() + source->m_id);
}

MixerSource::~MixerSource()
{
	if (m_engine == nullptr)
		return;

	m_engine->remove_source(this);
}

}  //namespace cog2d
