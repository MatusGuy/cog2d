// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <memory>
#include <qoa.h>

#include "cog2d/util/timing.hpp"
#include "cog2d/util/parsing.hpp"
#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/audio/audioengine.hpp"

namespace cog2d {

class MusicPlayer;

template<class T>
class Parser;

enum MusicType
{
	MUSIC_QOA
};

struct MusicTrackSection
{
	std::uint32_t start = 0;
	float bpm;
	std::uint32_t loop_start = 0;
	std::uint32_t end;
};

struct MusicTrackMetadata
{
	std::size_t start_section = 0;
	std::vector<MusicTrackSection> sections;
};

class MusicTrack
{
	friend class Parser<MusicTrack>;
	friend class MusicPlayer;

public:
	void* m_data;
	MusicType m_type;
	AudioSpec m_spec;
	std::unique_ptr<IoDevice> m_device;
	MusicTrackMetadata m_metadata;

public:
	MusicTrack();
	~MusicTrack();

	void load(toml::table& data);

	inline MusicTrackSection* section(std::size_t sec)
	{
		return sec < m_metadata.sections.size() ? &m_metadata.sections[sec] : nullptr;
	}

	std::size_t buffer(void* chunk, std::size_t chunk_size);
	void seek(std::size_t sample_frame);

private:
	struct MusicQoaData
	{
		unsigned char* qoa_data;
		std::size_t qoa_size;
		qoa_desc desc;

		~MusicQoaData() { delete[] qoa_data; }
	};

	std::size_t buffer_qoa(void* chunk, std::size_t chunk_size);

	void load_source(std::unique_ptr<IoDevice> device);
	void load_section(toml::table& data, MusicTrackSection& result, std::uint32_t samplerate);
};

}  //namespace cog2d
