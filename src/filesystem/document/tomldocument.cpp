#include "tomldocument.hpp"

#include <optional>

#include "cog2d/util/fmt.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TomlDocument::TomlDocument(toml::node& node, bool root)
    : DocumentNode(root),
      m_node(std::move(node))
{
}

DocumentNode::ValueType TomlDocument::valuetype_from_tomltype(toml::node_type t)
{
	switch (t) {
	case toml::node_type::integer:
		return DocumentNode::VALUE_INT;
	case toml::node_type::floating_point:
		return DocumentNode::VALUE_FLOAT;
	case toml::node_type::string:
		return DocumentNode::VALUE_STRING;
	case toml::node_type::boolean:
		return DocumentNode::VALUE_BOOL;
	case toml::node_type::array:
		return DocumentNode::VALUE_ARRAY;
	default:
	case toml::node_type::table:
		return DocumentNode::VALUE_DICT;
	}
}

void TomlDocument::load(IoDevice&& device)
{
	m_node = toml::parse(device);
}

std::unique_ptr<DocumentNode> TomlDocument::get(std::string_view key)
{
	toml::node_view<toml::node> node = m_node[key];

	if (node.node() == nullptr) {
		// TODO: custom exception for errors like these
		throw std::runtime_error(fmt::format("Could not find key '{}'", key));
	}

	return std::unique_ptr<DocumentNode>(new TomlDocument(*node.node()));
}

std::unique_ptr<DocumentNode> TomlDocument::get(int key)
{
	if (!m_node.is_array()) {
		throw std::runtime_error(fmt::format("Node is not of type array"));
	}

	toml::node_view<toml::node> node(m_node.as_array()->get(key));

	if (node.node() == nullptr) {
		throw std::runtime_error(fmt::format("Could not find index '{}'", key));
	}

	return std::unique_ptr<DocumentNode>(new TomlDocument(*node.node()));
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

DocumentNode::ValueType TomlDocument::type()
{
	return valuetype_from_tomltype(m_node.type());
}

COG2D_NAMESPACE_END_IMPL
