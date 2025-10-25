// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "file.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/util/fmt.hpp"

namespace cog2d {

File File::from_asset(std::string_view path)
{
	File file(fmt::format(COG2D_ASSET_PATH "/{}", path));
	return file;
	// TODO: Symlink detection logic
}

File::File()
    : m_path(),
      m_file(nullptr)
{
}

File::File(const std::string& path)
    : m_path(path),
      m_file(nullptr)
{
}

File::~File()
{
	close();
}

int File::open(const char* mode)
{
	if (is_open()) {
		seek(0, SEEK_SET);
	}

	m_file = std::fopen(m_path.c_str(), mode);
	return std::ferror(m_file);
}

bool File::is_open()
{
	return m_file != nullptr;
}

std::int64_t File::size()
{
	std::int64_t original = tell();

	seek(0, SEEK_SET);
	seek(0, SEEK_END);
	std::int64_t length = tell();

	seek(original, SEEK_SET);

	return length;
}

std::int64_t File::seek(std::int64_t offset, SeekPos seekpos)
{
	std::fseek(m_file, offset, seekpos);
	return tell();
}

std::int64_t File::tell()
{
	return std::ftell(m_file);
}

std::size_t File::read(void* ptr, std::size_t size, std::size_t maxnum)
{
	std::fread(static_cast<char*>(ptr), size, maxnum, m_file);
	return std::ftell(m_file);
}

std::size_t File::write(const void* ptr, std::size_t size, std::size_t num)
{
	std::fwrite(static_cast<const char*>(ptr), size, num, m_file);
	return num;
}

int File::close()
{
	std::fclose(m_file);
	return std::ferror(m_file);
}

bool File::eof()
{
	return std::feof(m_file) != 0;
}

SDL_RWops* File::to_sdl()
{
	return SDL_RWFromFP(m_file, false);
}

}  //namespace cog2d
