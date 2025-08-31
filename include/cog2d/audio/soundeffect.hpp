// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/audio/audioengine.hpp"

namespace cog2d {

class SoundEffect : public MixerSource
{
public:
	std::uint8_t* m_data = nullptr;
	std::uint32_t m_size;

	/// in sample frames
	std::size_t m_pos = std::numeric_limits<std::size_t>::max();

public:
	SoundEffect();
	~SoundEffect() override;

	void load(IoDevice& device);

	bool buffer(void* buf, std::size_t samples) override;

	bool is_playing();
	void play();
};

}  //namespace cog2d
