// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/filesystem/iodevice.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class DocumentNode
{
public:
	DocumentNode();

	enum ValueType
	{
		VALUE_INT,
		VALUE_FLOAT,
		VALUE_STRING,
		VALUE_BOOL,
		VALUE_ARRAY,
		VALUE_DICT
	};

	virtual void load(IoDevice&& device) = 0;

	virtual std::unique_ptr<DocumentNode> get(std::string_view key) = 0;
	virtual std::unique_ptr<DocumentNode> get(int key) = 0;

	virtual int as_int() = 0;
	virtual float as_float() = 0;
	virtual std::string as_string() = 0;
	virtual bool as_bool() = 0;

	virtual bool contains(std::string_view key) = 0;
	virtual ValueType type() = 0;
	virtual bool is_root() { return m_root; }

protected:
	DocumentNode(bool root)
	    : m_root(root)
	{
	}
	bool m_root;
};

COG2D_NAMESPACE_END_DECL
