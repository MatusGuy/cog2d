// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/filesystem/file.hpp"
#include "cog2d/audio/audioengine.hpp"

namespace cog2d {

class SoundEffect
{
public:
	std::uint8_t* data = nullptr;
	std::uint32_t size;
	AudioSpec spec;
	void* enginedata = nullptr;

public:
	SoundEffect();
	~SoundEffect();

	void load(File& device);
};

}  //namespace cog2d
