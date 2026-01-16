#pragma once

#include <cinttypes>
#include <string>
#include <vector>

namespace cog2d {

enum CmdlineArgType : std::uint8_t
{
	CMDLINE_SWITCH,
	CMDLINE_STRING,
	CMDLINE_INT,
	CMDLINE_DOUBLE,

	CMDLINE_COUNT
};

struct CmdlineArg
{
	std::string name = "";
	char shortname = '\0';
	std::string description = "";

	union
	{
		bool* b;
		std::string* s;
		int* i;
		double* d;
	} value = {(bool*) nullptr};
	CmdlineArgType type = CMDLINE_SWITCH;
};

struct CmdlineArgs
{
	std::vector<CmdlineArg> options;
	std::vector<CmdlineArg> required;
	CmdlineArg variadic;
};

int cmdline_parse(const int argc, char** const argv, const CmdlineArgs args);
int cmdline_parse_option(char* arg, CmdlineArgs const& args);
int cmdline_parse_string(const CmdlineArg& arg);

}  //namespace cog2d
