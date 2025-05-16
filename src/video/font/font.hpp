#ifndef FONT_HPP
#define FONT_HPP

#include <memory>
#include <string_view>

#include "cog2d/util/math/vector.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Texture;

class Font
{
public:
	virtual int get_text_width(std::string_view text) = 0;
	virtual void write_text(Texture* texture, std::string_view text,
	                        const Vector& pos = {0, 0}) = 0;
	virtual std::unique_ptr<Texture> create_text(std::string_view text) = 0;
};

COG2D_NAMESPACE_END_DECL

#endif  // FONT_HPP
