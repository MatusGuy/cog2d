// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdio>
#include <string>

struct SDL_RWops;

namespace cog2d {

#define COG2D_FILE_TEMPLATE(defaultfile) \
	template<class F = defaultfile, class = std::enable_if_t<std::is_base_of_v<File, F>>>

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

	SDL_RWops* to_sdl();
};

}  //namespace cog2d
