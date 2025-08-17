// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/math/vector.hpp"

namespace cog2d {

class IoDevice;
class Surface;

class Texture
{
public:
	void* m_data;
	Vector_t<int> m_size;

public:
	static Texture* from_surface(Surface& surface);
	static Texture* create(const Vector_t<int>& size);
	static Texture* from_pixmap(IoDevice& device);

public:
	Texture(void* tex);
	virtual ~Texture() {}

	inline void* data() { return m_data; }

	inline Vector_t<int> size()
	{
		if (m_data != nullptr && m_size.x == 0 && m_size.y == 0)
			m_size = query_size();

		return m_size;
	}

	virtual Vector_t<int> query_size() = 0;

protected:
	Texture(const Vector_t<int>& size);
	virtual bool construct() = 0;
};

}  //namespace cog2d
