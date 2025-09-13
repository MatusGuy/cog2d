#pragma once

#include <cstdint>
#include <algorithm>
#include <cstring>

namespace cog2d {

/// think of it as a fixed size stream
template<bool IsOwner = true>
struct Buffer
{
	constexpr static bool is_owner = IsOwner;

	unsigned char* data;
	std::size_t size;
	std::size_t pos = 0;

public:
	explicit Buffer(std::size_t sz)
	    : size(sz),
	      data(new std::uint8_t[sz]),
	      pos(0)
	{
		static_assert(is_owner, "");
	}

	Buffer(unsigned char* d, std::size_t sz)
	    : data(d),
	      size(sz),
	      pos(0)
	{
	}

	~Buffer()
	{
		if (is_owner && data != nullptr)
			delete[] data;
	}

	inline unsigned char* ptr() { return data + pos; }

	void seek(std::size_t p) { pos = std::clamp(p, static_cast<std::size_t>(0), size); }

	std::size_t read(unsigned char* out, std::size_t sz)
	{
		sz = std::min(sz, size - pos);

		std::memcpy(out, ptr(), sz);
		seek(pos + sz);
		return sz;
	}

	void swap(Buffer<IsOwner>& other)
	{
		std::swap(data, other.data);
		std::swap(size, other.size);
		std::swap(pos, other.pos);
	}
};

}  //namespace cog2d
