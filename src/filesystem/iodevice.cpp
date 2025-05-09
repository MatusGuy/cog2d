#include "iodevice.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

IoDevice::~IoDevice()
{
	if (is_open())
		close();
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
