// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <cstdint>
#include <string>

#include "cog2d/util/math/rect.hpp"
#include "cog2d/video/color.hpp"

namespace cog2d {

enum PropertyType : std::uint8_t
{
	PROPTYPE_NONE,
	PROPTYPE_INT,
	PROPTYPE_BOOL,
	PROPTYPE_FLOAT,
	PROPTYPE_STRING,
	PROPTYPE_VECTOR,
	PROPTYPE_VECTORI,
	PROPTYPE_RECT,
	PROPTYPE_RECTI,
	PROPTYPE_COLOR
};

struct Property
{
	PropertyType type = PROPTYPE_NONE;
	union
	{
		void* any;
		std::int32_t* i;
		bool* b;
		float* f;
		std::string* s;
		Vector* v;
		Vector_t<std::int32_t>* vi;
		Rect* r;
		Rect_t<std::int32_t>* ri;
		Color* c;
	};
};

struct CompProperties
{
	Property data[10];

	inline void add_property(int idx, PropertyType type, void* p) {
		data[idx].type = type;
		data[idx].any = p;
	}
};

}  //namespace cog2d
