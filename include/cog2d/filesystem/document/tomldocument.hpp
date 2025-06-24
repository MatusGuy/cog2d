#pragma once

#include <toml++/toml.hpp>

#include "cog2d/filesystem/document/documentnode.hpp"

namespace toml {
inline auto parse(cog2d::IoDevice&& stream)
{
	// NOTE: I don't like all this moving i'm doing...
	return parse(*stream.stl_stream());
}
}  //namespace toml

COG2D_NAMESPACE_BEGIN_DECL

class TomlDocument : public DocumentNode
{
public:
	static ValueType valuetype_from_tomltype(toml::node_type t);

public:
	void load(IoDevice&& device) override;

	std::unique_ptr<DocumentNode> get(std::string_view key) override;
	std::unique_ptr<DocumentNode> get(int key) override;

	int as_int() override;
	float as_float() override;
	std::string as_string() override;
	bool as_bool() override;

	bool contains(std::string_view key) override;
	ValueType type() override;

private:
	TomlDocument(toml::node& node, bool root = false);

	toml::node_view<toml::node> m_node;
};

COG2D_NAMESPACE_END_DECL
