#include "file.hpp"

#include <iostream>
#include <ios>

COG2D_NAMESPACE_BEGIN_IMPL

File::File(const std::filesystem::path& path)
    : m_path(path),
      m_stream()
{
}

int File::open(OpenMode mode)
{
	m_stream.open(m_path, mode);

	return m_stream.good() ? 0 : -1;
}

bool File::is_open()
{
	return m_stream.is_open();
}

std::int64_t File::size()
{
	std::streampos original = tell();

	seek(0, std::ios_base::beg);
	m_stream.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize length = m_stream.gcount();

	m_stream.clear();
	seek(original, std::ios_base::beg);

	return length;
}

std::int64_t File::seek(int64_t offset, SeekPos seekpos)
{
	m_stream.seekg(static_cast<std::streamoff>(offset), seekpos);
	return tell();  // ...right?
}

std::int64_t File::tell()
{
	return m_stream.tellg();
}

std::size_t File::read(void* ptr, std::size_t, std::size_t maxnum)
{
	m_stream.read(reinterpret_cast<char*>(ptr), maxnum);
	return m_stream.gcount();
}

std::size_t File::write(const void* ptr, std::size_t size, std::size_t num)
{
	m_stream.write(reinterpret_cast<const char*>(ptr), num);
	return num;
}

int File::close()
{
	m_stream.close();
	return m_stream.good() ? 0 : -1;
}

COG2D_NAMESPACE_END_IMPL
