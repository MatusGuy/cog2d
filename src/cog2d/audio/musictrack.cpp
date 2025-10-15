// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musictrack.hpp"

#include <cstring>
#include <thread>

#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/util/logger.hpp"

#include <qoa.h>

namespace cog2d {

MusicTrack::MusicTrack()
    : m_data(nullptr),
      m_type(MUSIC_QOA),
      m_spec(),
      m_device(),
      m_metadata()
{
}

MusicTrack::~MusicTrack()
{
	if (m_device)
		m_device->close();

	switch (m_type) {
	case MUSIC_QOA:
		delete static_cast<MusicQoaData*>(m_data);
		break;
	}
}

void MusicTrack::load_source(std::unique_ptr<IoDevice> device)
{
	m_device = std::move(device);
	m_device->open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	std::size_t filesz = m_device->seek(0, IoDevice::SEEKPOS_END);
	m_device->seek(0, IoDevice::SEEKPOS_BEGIN);

	MusicQoaData* data = new MusicQoaData;
	m_data = static_cast<void*>(data);

	unsigned char header[QOA_MIN_FILESIZE];
	m_device->read(header, sizeof(unsigned char), QOA_MIN_FILESIZE);
	qoa_decode_header(header, QOA_MIN_FILESIZE, &data->desc);
	m_spec.channels = data->desc.channels;
	m_spec.samplerate = data->desc.samplerate;
	m_spec.format = AUDIOFORMAT_S16_LE;
	m_device->seek(-8, IoDevice::SEEKPOS_CURSOR);

	//data->qoa_size = qoa_max_frame_size(&data->desc) * std::ceil(data->desc.samples / (256 * 20));
	data->qoa_size = filesz - 8;
	data->qoa_data = new unsigned char[data->qoa_size];
	m_device->read(data->qoa_data, sizeof(unsigned char), data->qoa_size);

	/*
	m_size = data->desc.samples * data->desc.channels;
	m_music = static_cast<void*>(new std::int16_t[m_size]);

	std::size_t p;
	unsigned int sample_index = 0;
	unsigned int frame_len;
	unsigned int frame_size;

	const std::size_t framesz = qoa_max_frame_size(&desc);
	unsigned char* frame = new unsigned char[framesz];
	// Decode all frames
	do {
		m_device->read(frame, sizeof(unsigned char), framesz);

		short* sample_ptr = static_cast<short*>(m_music) + sample_index * desc.channels;
		frame_size = qoa_decode_frame(frame, framesz, &desc, sample_ptr, &frame_len);

		p += frame_size;
		sample_index += frame_len;
	} while (frame_size && sample_index < desc.samples && !m_device->eof());

	delete[] frame;
	*/
}

}  //namespace cog2d
