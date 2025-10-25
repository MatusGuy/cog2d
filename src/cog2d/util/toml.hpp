// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <tomlc.h>

#include "cog2d/filesystem/file.hpp"
#include "cog2d/util/fmt.hpp"
#include "cog2d/util/typetraits.hpp"

namespace cog2d {

struct TomlTable;

struct TomlArray
{
	toml_array_t* data;

	TomlArray(toml_array_t* d = nullptr)
	    : data(d)
	{
	}

	int len() { return toml_array_len(data); }

	template<typename I>
	int at(int idx, I& out)
	{
		toml_value_t v = toml_array_int(data, idx);
		if (!v.ok)
			return 1;

		out = v.u.i;

		return 0;
	}

	int at(int idx, bool& out)
	{
		toml_value_t v = toml_array_bool(data, idx);
		if (!v.ok)
			return 1;

		out = v.u.b;

		return 0;
	}

	int at(int idx, double& out)
	{
		toml_value_t v = toml_array_double(data, idx);
		if (!v.ok)
			return 1;

		out = v.u.d;

		return 0;
	}

	int at(int idx, std::string& out)
	{
		toml_value_t v = toml_array_string(data, idx);
		if (!v.ok)
			return 1;

		out.replace(0, out.length(), v.u.s, v.u.sl);
		std::free(v.u.s);

		return 0;
	}

	int at(int idx, TomlTable& out);

	int at(int idx, TomlArray& out);
};

struct TomlTable
{
	toml_table_t* data = nullptr;

	TomlTable(toml_table_t* tbl = nullptr)
	    : data(tbl)
	{
	}

	int len() { return toml_table_len(data); }

	std::string key_at(int idx)
	{
		int len;
		const char* str = toml_table_key(data, idx, &len);
		return std::string(str, len);
	}

	template<typename I>
	int at(std::string_view key, I& out)
	{
		toml_value_t v = toml_table_int(data, key.data());
		if (!v.ok)
			return 1;

		out = v.u.i;

		return 0;
	}

	int at(std::string_view key, bool& out)
	{
		toml_value_t v = toml_table_bool(data, key.data());
		if (!v.ok)
			return 1;

		out = v.u.b;

		return 0;
	}

	int at(std::string_view key, double& out)
	{
		toml_value_t v = toml_table_double(data, key.data());
		if (!v.ok)
			return 1;

		out = v.u.d;

		return 0;
	}

	int at(std::string_view key, std::string& out)
	{
		toml_value_t v = toml_table_string(data, key.data());
		if (!v.ok)
			return 1;

		out.replace(0, out.length(), v.u.s, v.u.sl);
		std::free(v.u.s);

		return 0;
	}

	int at(std::string_view key, TomlTable& out);

	int at(std::string_view key, TomlArray& out);
};

struct TomlResult
{
	TomlTable tbl;
	char error[100] = {0};

	TomlResult(toml_table_t* d = nullptr)
	    : tbl(d)
	{
	}

	~TomlResult()
	{
		if (tbl.data)
			toml_free(tbl.data);
	}
};

TomlResult toml_parse(File& device);

}  //namespace cog2d
