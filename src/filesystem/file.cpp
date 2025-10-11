// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "file.hpp"

#include "cog2d/util/logger.hpp"

#include <iostream>
#include <ios>

namespace cog2d {

File::File(const std::filesystem::path& path)
    : m_path(path),
      m_stream()
{
	m_path.make_preferred();
	m_stream.exceptions(std::ios::badbit);
}

int File::open(OpenMode mode)
{
	m_stream.open(m_path, static_cast<std::ios::openmode>(mode));

	return m_stream.good() ? 0 : -1;
}

bool File::is_open()
{
	return m_stream.is_open();
}

std::int64_t File::size()
{
	std::streampos original = tell();

	seek(0, SEEKPOS_BEGIN);
	m_stream.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize length = m_stream.gcount();

	m_stream.clear();
	seek(original, SEEKPOS_BEGIN);

	return length;
}

std::int64_t File::seek(std::int64_t offset, SeekPos seekpos)
{
	m_stream.seekg(static_cast<std::streamoff>(offset), static_cast<std::ios::seekdir>(seekpos));
	//m_stream.clear();
	return tell();
}

std::int64_t File::tell()
{
	return m_stream.tellg();
}

std::size_t File::read(void* ptr, std::size_t size, std::size_t maxnum)
{
	m_stream.read(static_cast<char*>(ptr), size * maxnum);
	return m_stream.gcount();
}

std::size_t File::write(const void* ptr, std::size_t size, std::size_t num)
{
	m_stream.write(static_cast<const char*>(ptr), size * num);
	return num;
}

int File::close()
{
	m_stream.close();
	return m_stream.good() ? 0 : -1;
}

}
