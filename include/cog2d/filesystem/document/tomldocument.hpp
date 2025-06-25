#pragma once

#include <iostream>

#include <toml++/toml.hpp>

#include "cog2d/filesystem/iodevice.hpp"

namespace toml {
inline auto parse(cog2d::IoDevice& stream)
{
	std::istream* ptr = stream.stl_stream();
	return parse(*ptr);
}
}  //namespace toml

COG2D_NAMESPACE_BEGIN_DECL

class TomlDocument
{
public:
	TomlDocument();
	TomlDocument(toml::node& node, bool root = true);
	TomlDocument(toml::node&& node, bool root = true);

	void load(IoDevice& device);

	TomlDocument get(std::string_view key);
	TomlDocument get(int key);

	int as_int();
	float as_float();
	std::string as_string();
	bool as_bool();

	bool contains(std::string_view key);
	toml::node_type type();

	toml::array::iterator abegin();
	toml::array::iterator aend();

	toml::table::iterator dbegin();
	toml::table::iterator dend();

	TomlDocument operator[](std::string_view key) { return get(key); }
	TomlDocument operator[](int key) { return get(key); }

private:
	toml::node_view<toml::node> m_node;
	bool m_root;
};

COG2D_NAMESPACE_END_DECL
