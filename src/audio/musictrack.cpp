// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musictrack.hpp"

#include <cstring>

#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/util/logger.hpp"

#include <qoa.h>

namespace cog2d {

struct MusicQoaBuffer
{
	unsigned char* qoa_data;
	std::size_t qoa_size;
	qoa_desc desc;

	/// current sample frame
	std::size_t qoa_pos = 0;

	/// currently decoded qoa frame
	short* frame;
	std::size_t frame_size = 0;
	std::size_t frame_pos = std::numeric_limits<std::size_t>::max();

	~MusicQoaBuffer()
	{
		delete[] qoa_data;
		delete[] frame;
	}
};

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
		delete static_cast<MusicQoaBuffer*>(m_data);
		break;
	}
}

void MusicTrack::load(std::unique_ptr<IoDevice> device)
{
	m_device = std::move(device);
	m_device->open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	std::size_t filesz = m_device->seek(0, IoDevice::SEEKPOS_END);
	m_device->seek(0, IoDevice::SEEKPOS_BEGIN);

	MusicQoaBuffer* data = new MusicQoaBuffer;
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

	data->frame_size = data->desc.channels * 20 * 256;
	data->frame = new short[data->frame_size];

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

std::size_t MusicTrack::buffer(void* chunk, std::size_t chunk_size)
{
	switch (m_type) {
	case MUSIC_QOA:
		return buffer_qoa(chunk, chunk_size);
	}
}

void MusicTrack::seek(std::size_t sample_frame)
{
}

std::size_t MusicTrack::buffer_qoa(void* chunk, std::size_t chunk_size)
{
	auto data = static_cast<MusicQoaBuffer*>(m_data);
	std::size_t chunk_pos = 0, frame_sz = 0, frame_len = 0;

	do {
		if (data->frame_pos >= data->frame_size) {
			std::size_t temp_len;
			frame_sz = qoa_decode_frame(data->qoa_data + data->qoa_pos, data->frame_size,
			                            &data->desc, data->frame, (unsigned int*) &temp_len);

			if (frame_sz == 0)
				return 0;

			frame_len += temp_len;

			data->frame_pos = 0;
			data->qoa_pos += frame_sz;
			COG2D_LOG_DEBUG(fmt::format("decode -- size: {}, pos: {}, samples: {}", frame_sz,
			                            data->qoa_pos, frame_len));
		};

		std::size_t cpysz = std::min(chunk_size, data->frame_size);
		std::memcpy(chunk + chunk_pos, data->frame + data->frame_pos, cpysz);

		chunk_pos += cpysz;
		data->frame_pos += cpysz / sizeof(short);
	} while (chunk_pos < chunk_size);

	return frame_len;
}

}  //namespace cog2d
