// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only
#include "toml.hpp"

#include "cog2d/util/logger.hpp"

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

int toml_write(File& device, const TomlWriteData data)
{
	device.open("w");

	device.write(fmt::format("type = \"{}\"\n", data.type));
	device.write(fmt::format("version = {}\n\n", data.version));

	std::string table = "";
	for (TomlWriteDataEntry& entry : data.values) {
		if (table != entry.table)
			device.write(fmt::format("[{}]\n\n"));

		if (entry.type == TOML_TIMESTAMP) {
			log::warn("Writing TOML timestamps is not supported yet.");
			continue;
		}

		device.write(fmt::format("{} = ", entry.key));
		switch (entry.type) {
		case TOML_INT:
			device.write(std::to_string(*((std::int64_t*) entry.value)));
			break;

		case TOML_DOUBLE:
			device.write(std::to_string(*((double*) entry.value)));
			break;

		case TOML_BOOL:
			device.write(*((bool*) entry.value) ? "true" : "false");

		case TOML_STRING:
			device.write(fmt::format("\"{}\"", *((std::string*) entry.value)));
			break;

		default:
			break;
		}
		device.write("\n");
	}
}

}  //namespace cog2d
