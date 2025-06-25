#include "tomldocument.hpp"

#include <optional>

#include "cog2d/util/fmt.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TomlDocument::TomlDocument()
    : m_node(nullptr),
      m_root(true)
{
}

TomlDocument::TomlDocument(toml::node& node, bool root)
    : m_node(std::move(node)),
      m_root(root)
{
}

TomlDocument::TomlDocument(toml::node&& node, bool root)
    : m_node(node),
      m_root(root)
{
}

void TomlDocument::load(IoDevice& device)
{
	m_node = toml::node_view<toml::node>(toml::parse(device));
}

TomlDocument TomlDocument::get(std::string_view key)
{
	toml::node_view<toml::node> node = m_node[key];

	if (node.node() == nullptr) {
		// TODO: custom exception for errors like these
		throw std::runtime_error(fmt::format("Could not find key '{}'", key));
	}

	return TomlDocument(*node.node(), false);
}

TomlDocument TomlDocument::get(int key)
{
	if (!m_node.is_array()) {
		throw std::runtime_error(fmt::format("Node is not of type array"));
	}

	toml::node_view<toml::node> node(m_node.as_array()->get(key));

	if (node.node() == nullptr) {
		throw std::runtime_error(fmt::format("Could not find index '{}'", key));
	}

	return TomlDocument(*node.node(), false);
}

int TomlDocument::as_int()
{
	std::optional<int> out = m_node.value<int>();
	if (!out.has_value())
		throw std::runtime_error(fmt::format("Node is not of type int"));

	return out.value();
}

float TomlDocument::as_float()
{
	std::optional<float> out = m_node.value<float>();
	if (!out.has_value())
		throw std::runtime_error(fmt::format("Node is not of type float"));

	return out.value();
}

std::string TomlDocument::as_string()
{
	std::optional<std::string> out = m_node.value<std::string>();
	if (!out.has_value())
		throw std::runtime_error(fmt::format("Node is not of type string"));

	return out.value();
}

bool TomlDocument::as_bool()
{
	std::optional<bool> out = m_node.value<bool>();
	if (!out.has_value())
		throw std::runtime_error(fmt::format("Node is not of type bool"));

	return out.value();
}

bool TomlDocument::contains(std::string_view key)
{
	return m_node[key].node() != nullptr;
}

toml::node_type TomlDocument::type()
{
	return m_node.type();
}

toml::array::iterator TomlDocument::abegin()
{
	if (!m_node.is_array()) {
		throw std::runtime_error(fmt::format("Node is not of type array"));
	}

	return m_node.as_array()->begin();
}

toml::array::iterator TomlDocument::aend()
{
	if (!m_node.is_array()) {
		throw std::runtime_error(fmt::format("Node is not of type array"));
	}

	return m_node.as_array()->end();
}

toml::table::iterator TomlDocument::dbegin()
{
	if (!m_node.is_table()) {
		throw std::runtime_error(fmt::format("Node is not of type dictionary"));
	}

	return m_node.as_table()->begin();
}

toml::table::iterator TomlDocument::dend()
{
	if (!m_node.is_table()) {
		throw std::runtime_error(fmt::format("Node is not of type dictionary"));
	}

	return m_node.as_table()->end();
}

COG2D_NAMESPACE_END_IMPL
