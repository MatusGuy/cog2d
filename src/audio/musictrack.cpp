// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musictrack.hpp"

#include "cog2d/filesystem/iodevice.hpp"

#include <qoa.h>

namespace cog2d {

MusicTrack::MusicTrack()
    : m_music(),
      m_size(),
      m_metadata()
{
}

MusicTrack::~MusicTrack()
{
	if (m_device)
		m_device->close();

	delete[] m_music;
}

void MusicTrack::load(std::unique_ptr<IoDevice> device)
{
	m_device = std::move(device);
	m_device->open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	unsigned char header[QOA_MIN_FILESIZE];
	m_device->read(header, sizeof(unsigned char), QOA_MIN_FILESIZE);
	qoa_desc desc;
	qoa_decode_header(header, QOA_MIN_FILESIZE, &desc);
	m_spec.channels = desc.channels;
	m_spec.samplerate = desc.samplerate;
	m_spec.format = AUDIOFORMAT_S16_LE;
	m_device->seek(-8, IoDevice::SEEKPOS_CURSOR);

	m_size = desc.samples * desc.channels;
	m_music = static_cast<void*>(new std::int16_t[m_size]);

	std::size_t p;
	unsigned int sample_index = 0;
	unsigned int frame_len;
	unsigned int frame_size;

	const std::size_t framesz = qoa_max_frame_size(&desc);
	unsigned char* frame = new unsigned char[framesz];
	/* Decode all frames */
	do {
		m_device->read(frame, sizeof(unsigned char), framesz);

		short* sample_ptr = static_cast<short*>(m_music) + sample_index * desc.channels;
		frame_size = qoa_decode_frame(frame, framesz, &desc, sample_ptr, &frame_len);

		p += frame_size;
		sample_index += frame_len;
	} while (frame_size && sample_index < desc.samples && !m_device->eof());

	delete[] frame;
}

}  //namespace cog2d
