// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <toml++/toml.hpp>

#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/util/fmt.hpp"

namespace toml {
inline auto parse(cog2d::IoDevice& stream)
{
	// Bitchy compiler wont accept anything but a reinterpret cast
	return parse(reinterpret_cast<std::istream&>(*stream.stl_stream()));
}
}  //namespace toml

namespace cog2d {

namespace toml_util {

template<class T>
inline T as(toml::node& node)
{
	if (!node.is<T>())
		throw std::runtime_error("Attempted to use node with wrong type");

	return node.value<T>().value();
}

inline toml::array& as_array(toml::node& node)
{
	if (!node.is_array())
		throw std::runtime_error("Attempted to use node with wrong type array");

	return *node.as_array();
}

inline toml::table& as_table(toml::node& node)
{
	if (!node.is_table())
		throw std::runtime_error("Attempted to use node with wrong type table");

	return *node.as_table();
}

inline toml::node& get(toml::node& node, std::string_view key)
{
	toml::table& table = as_table(node);

	if (!table.contains(key))
		throw std::runtime_error(fmt::format("Could not find key '{}'", key));

	return table.at(key);
}

template<class T>
inline T get_as(toml::node& node, std::string_view key)
{
	return as<T>(get(node, key));
}

inline toml::array& get_as_array(toml::node& node, std::string_view key)
{
	return as_array(get(node, key));
}

inline toml::array& get_as_table(toml::node& node, std::string_view key)
{
	return as_array(get(node, key));
}

}  //namespace toml_util

// TODO: Custom exception for parsing error
template<class T>
class Parser
{
	friend T;

public:
	/**
	 * @brief parse
	 * @throws parser exception
	 * @param device
	 * @param result
	 */
	virtual void parse(IoDevice& device, T& result) = 0;
};

}  //namespace cog2d
