// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only
#include "toml.hpp"

namespace cog2d {

int TomlArray::at(int idx, TomlTable& out)
{
	toml_table_t* v = toml_array_table(data, idx);
	if (!v)
		return 1;

	out = TomlTable(v);

	return 0;
}

int TomlArray::at(int idx, TomlArray& out)
{
	toml_array_t* v = toml_array_array(data, idx);
	if (!v)
		return 1;

	out = TomlArray(v);

	return 0;
}

int TomlTable::at(std::string_view key, TomlArray& out)
{
	toml_array_t* v = toml_table_array(data, key.data());
	if (!v)
		return 1;

	out = TomlArray(v);

	return 0;
}

int TomlTable::at(std::string_view key, TomlTable& out)
{
	toml_table_t* v = toml_table_table(data, key.data());
	if (!v)
		return 1;

	out = TomlTable(v);

	return 0;
}

TomlResult toml_parse(File& device)
{
	device.open("r");

	std::size_t size = device.size();

	char* toml = new char[size];
	device.read(toml, sizeof(char), size);

	device.close();

	TomlResult result;
	result.tbl = ::toml_parse(toml, result.error, sizeof(result.error));

	delete[] toml;
	return result;
}

}  //namespace cog2d
