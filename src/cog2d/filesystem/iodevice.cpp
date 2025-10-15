// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "iodevice.hpp"

namespace cog2d {

IoDevice::~IoDevice()
{
}

std::int64_t IoDevice::tell()
{
	return seek(0, SEEKPOS_CURSOR);
}

bool IoDevice::eof()
{
	return tell() == size();
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

}
