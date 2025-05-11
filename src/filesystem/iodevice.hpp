#ifndef IODEVICE_H
#define IODEVICE_H

#include <cstdio>
#include <ios>

#include <SDL2/SDL_rwops.h>

#include "cog2d/util/types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class IoDevice
{
public:
	enum SeekPos
	{
		SEEKPOS_BEGIN = std::ios::beg,
		SEEKPOS_CURSOR = std::ios::cur,
		SEEKPOS_END = std::ios::end
	};

	enum
	{
		OPENMODE_READ = std::ios::in,
		OPENMODE_WRITE = std::ios::out,
		OPENMODE_BINARY = std::ios::binary
	};
	using OpenMode = std::uint16_t;

public:
	~IoDevice();

	virtual int open(OpenMode mode) = 0;
	virtual bool is_open() = 0;
	virtual std::int64_t size() = 0;
	virtual std::int64_t seek(std::int64_t offset, SeekPos seekpos) = 0;
	virtual std::int64_t tell() = 0;
	virtual std::size_t read(void* ptr, std::size_t size, std::size_t maxnum) = 0;
	virtual std::size_t write(const void* ptr, std::size_t size, std::size_t num) = 0;
	virtual int close() = 0;

	virtual SDL_RWops* to_sdl();

	static std::int64_t sdl_size(SDL_RWops* context)
	{
		auto dev = static_cast<IoDevice*>(context->hidden.unknown.data1);
		return dev->size();
	}
	static std::int64_t sdl_seek(SDL_RWops* context, std::int64_t offset, int seekpos)
	{
		auto dev = static_cast<IoDevice*>(context->hidden.unknown.data1);
		std::int64_t r = dev->seek(offset, static_cast<SeekPos>(seekpos));
		return r;
	}
	static std::size_t sdl_read(SDL_RWops* context, void* ptr, std::size_t size, std::size_t maxnum)
	{
		auto dev = static_cast<IoDevice*>(context->hidden.unknown.data1);
		return dev->read(ptr, size, maxnum);
	}
	static std::size_t sdl_write(SDL_RWops* context, const void* ptr, std::size_t size,
	                             std::size_t num)
	{
		auto dev = static_cast<IoDevice*>(context->hidden.unknown.data1);
		return dev->write(ptr, size, num);
	}
	static int sdl_close(SDL_RWops* context)
	{
		auto dev = static_cast<IoDevice*>(context->hidden.unknown.data1);
		return dev->close();
	}
};

COG2D_NAMESPACE_END_DECL

#endif  // IODEVICE_H
