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

public:
	Texture(void* tex);
	~Texture();

	inline void* data() { return m_data; }

	inline Vector get_size() { return m_size; }
	virtual Vector_t<int> query_size();
};

}  //namespace cog2d
