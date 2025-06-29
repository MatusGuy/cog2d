// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <string_view>

#include "cog2d/util/math/vector.hpp"

namespace cog2d {

class Texture;

class Font
{
public:
	virtual int get_text_width(std::string_view text) = 0;
	virtual void write_text(Texture* texture, std::string_view text,
	                        const Vector& pos = {0, 0}) = 0;
	virtual std::unique_ptr<Texture> create_text(std::string_view text) = 0;
};

}
