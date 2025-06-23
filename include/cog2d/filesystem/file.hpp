// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <fstream>
#include <filesystem>

#include "cog2d/filesystem/iodevice.hpp"

COG2D_NAMESPACE_BEGIN_DECL

#define COG2D_FILE_TEMPLATE(defaultfile) \
	template<class F = defaultfile, class = std::enable_if_t<std::is_base_of_v<File, F>>>

class File : public IoDevice
{
public:
	std::filesystem::path m_path;
	std::fstream m_stream;

public:
	File(const std::filesystem::path& path);

	int open(OpenMode mode) override;
	bool is_open() override;
	std::int64_t size() override;
	std::int64_t seek(int64_t offset, SeekPos seekpos) override;
	std::int64_t tell() override;
	std::size_t read(void* ptr, std::size_t size, std::size_t maxnum) override;
	std::size_t write(const void* ptr, std::size_t size, std::size_t num) override;
	int close() override;

	std::iostream* stl_stream() override { return &m_stream; }
};

COG2D_NAMESPACE_END_DECL
