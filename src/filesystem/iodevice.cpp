// SPDX-License-Identifier: LGPL-3.0-only

#include "iodevice.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

IoDevice::~IoDevice()
{
}

std::int64_t IoDevice::tell()
{
	return seek(0, SEEKPOS_CURSOR);
}

bool IoDevice::eof()
{
	uint8_t a;

	try {
		read(&a, 1, 1);
	} catch (...) {
		// TODO: check specifically for eof exception
		return true;
	}

	seek(-1, SEEKPOS_CURSOR);

	return false;
}

SDL_RWops* IoDevice::to_sdl()
{
	auto result = new SDL_RWops;

	result->hidden.unknown.data1 = static_cast<void*>(this);
	result->size = &sdl_size;
	result->close = &sdl_close;
	result->read = &sdl_read;
	result->write = &sdl_write;
	result->seek = &sdl_seek;

	return result;
}

COG2D_NAMESPACE_END_IMPL
