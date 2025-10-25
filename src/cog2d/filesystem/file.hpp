// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdio>
#include <cstdint>
#include <string>

struct SDL_RWops;

namespace cog2d {

class File
{
public:
	using SeekPos = std::uint8_t;

public:
	std::string m_path;
	std::FILE* m_file;

public:
	static File from_asset(std::string_view path);

public:
	File();
	File(const std::string& path);
	~File();

	int open(const char* mode);
	bool is_open();
	std::int64_t size();
	std::int64_t seek(std::int64_t offset, SeekPos seekpos);
	std::int64_t tell();
	std::size_t read(void* ptr, std::size_t size, std::size_t maxnum);
	std::size_t write(const void* ptr, std::size_t size, std::size_t num);
	int close();
	bool eof();

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

	SDL_RWops* to_sdl();
};

}  //namespace cog2d
