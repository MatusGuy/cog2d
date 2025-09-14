// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musicplayer.hpp"

#include <functional>
#include <thread>
#include <cstring>
#include <cassert>

#include <qoa.h>

#include "cog2d/audio/musictrack.hpp"
#include "cog2d/util/buffer.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

MusicPlayer::MusicPlayer()
{
}

void MusicPlayer::init()
{
	using namespace std::placeholders;

	m_track = nullptr;

	COG2D_USE_AUDIOENGINE;
	audioengine.add_source(this);
}

void MusicPlayer::deinit()
{
}

/* QOA */

struct MusicPlayer::MusicQoaBuffer
{
	enum State : std::uint8_t
	{
		STATE_PLAYING,
		STATE_LOOP_PENDING
	};
	State state = STATE_PLAYING;
	std::uint32_t pending_track_pos = 0;

	MusicTrack::MusicQoaData* music_data;

	/// current position in the qoa file
	Buffer<false> qoa_data;

	/// current qoa frame being decoded
	Buffer<> ready_frame;
	//std::thread decode_thread;

	/// currently playing decoded qoa frame
	Buffer<> frame;

	static void decode_frame(MusicQoaBuffer* buf);
	void seek(std::uint32_t sample_frame);

	MusicQoaBuffer(MusicTrack::MusicQoaData* data)
	    : music_data(data),
	      qoa_data(data->qoa_data, data->qoa_size),
	      ready_frame(data->desc.channels * QOA_FRAME_LEN * sizeof(short)),
	      frame(ready_frame.size)
	//decode_thread(decode_frame, this),
	{
	}
};

void MusicPlayer::load_qoa(MusicTrack* track)
{
	auto data = static_cast<MusicTrack::MusicQoaData*>(track->m_data);
	auto buf = new MusicQoaBuffer(data);

	unsigned int readsz, readlen;

	readsz = qoa_decode_frame(buf->qoa_data.ptr(), buf->frame.size, &data->desc,
	                          (short*) buf->frame.data, &readlen);
	buf->qoa_data.seek(buf->qoa_data.pos + readsz);
	readsz = qoa_decode_frame(buf->qoa_data.ptr(), buf->ready_frame.size, &data->desc,
	                          (short*) buf->ready_frame.data, &readlen);
	buf->qoa_data.seek(buf->qoa_data.pos + readsz);

	m_buffer_data = buf;
}

void MusicPlayer::buffer_qoa(MusicQoaBuffer* buf, unsigned char* out, std::size_t size)
{
	assert(size <= buf->frame.size);

	std::size_t readsz = buf->frame.read(out, size);

	if (buf->state == MusicQoaBuffer::STATE_PLAYING)
		m_track_pos += readsz / m_spec.channels / m_spec.size();

	if (readsz >= size)
		// Haven't reached the end of the buffer yet.
		return;

	switch (buf->state) {
	case MusicQoaBuffer::STATE_PLAYING: {
		const std::uint32_t tracksample = m_track_pos + (readsz / m_spec.channels / m_spec.size());
		if ((tracksample / static_cast<double>(m_spec.samplerate)) >= m_current_section->end) {
			buf->state = MusicQoaBuffer::STATE_LOOP_PENDING;
			buf->pending_track_pos = m_current_section->loop_start;
			buf->seek(buf->pending_track_pos);
		}
		break;
	}

	case MusicQoaBuffer::STATE_LOOP_PENDING:
		m_track_pos = buf->pending_track_pos;
		buf->state = MusicQoaBuffer::STATE_PLAYING;
		break;

	default:
		break;
	}

	buf->frame.pos = 0;
	buf->frame.swap(buf->ready_frame);
	std::thread decode_thread(MusicQoaBuffer::decode_frame, buf);
	decode_thread.detach();
	buf->frame.read(out + readsz, size - readsz);
	m_track_pos += (size - readsz) / m_spec.channels / m_spec.size();
}

void MusicPlayer::MusicQoaBuffer::seek(std::uint32_t sample_frame)
{
	qoa_data.pos = std::floor(sample_frame / QOA_FRAME_LEN) * qoa_max_frame_size(&music_data->desc);
}

void MusicPlayer::MusicQoaBuffer::decode_frame(MusicQoaBuffer* buf)
{
	unsigned int readsz, readlen;
	readsz = qoa_decode_frame(buf->qoa_data.ptr(), buf->ready_frame.size, &buf->music_data->desc,
	                          (short*) buf->ready_frame.data, &readlen);
	buf->qoa_data.seek(buf->qoa_data.pos + readsz);
}

/* Control */

bool MusicPlayer::buffer(void* buf, std::size_t samples)
{
	if (m_track == nullptr)
		return false;

	switch (m_track->m_type) {
	case MUSIC_QOA:
		buffer_qoa(static_cast<MusicQoaBuffer*>(m_buffer_data), static_cast<unsigned char*>(buf),
		           m_spec.samples_to_bytes(samples));
		break;

	default:
		break;
	}

	COG2D_LOG_DEBUG(fmt::format("{} {}", m_track_pos / static_cast<double>(m_spec.samplerate),
	                            m_current_section->end));

	//m_track_pos += samples;
	return true;
}

void MusicPlayer::set_track(MusicTrack* track)
{
	if (m_track == track)
		return;

	if (m_buffer_data != nullptr) {
		switch (m_track->m_type) {
		case MUSIC_QOA:
			delete static_cast<MusicQoaBuffer*>(m_buffer_data);
			break;

		default:
			break;
		}
	}

	m_track = track;

	switch (m_track->m_type) {
	case MUSIC_QOA:
		load_qoa(m_track);
		break;

	default:
		break;
	}

	m_spec = track->m_spec;
	m_current_section = track->section(track->m_metadata.start_section);
	m_next_section = m_current_section;
	AudioEngine::get().refresh_source(this);
}

void MusicPlayer::queue_section(std::size_t section)
{
	queue_section(m_track->section(section));
}

void MusicPlayer::queue_section(MusicTrackSection* section)
{
	m_next_section = section;
}

}  //namespace cog2d
