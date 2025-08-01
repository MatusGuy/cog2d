// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdint>
#include <memory>
#include <ios>

#include <SDL2/SDL_rwops.h>

namespace cog2d {

// TODO: inherit iostream? compose of an iostream? something like that.
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
	virtual std::size_t read(void* ptr, std::size_t size, std::size_t maxnum) = 0;
	virtual std::size_t write(const void* ptr, std::size_t size, std::size_t num) = 0;
	virtual int close() = 0;

	virtual std::int64_t tell();
	virtual bool eof();

	template<class T>
	inline std::size_t read(T& ref, std::size_t maxnum = 1)
	{
		return read(static_cast<void*>(&ref), sizeof(T), maxnum);
	}

	template<class T>
	inline std::size_t read(std::basic_string<T>& ref)
	{
		T c;
		while (true) {
			read(c);
			if (c == 0)
				break;
			ref += c;
		}

		return ref.size() + 1;
	}

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

	virtual std::iostream* stl_stream() { return nullptr; }
};

}
