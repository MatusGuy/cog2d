#pragma once

#include <cinttypes>
#include <string>
#include <vector>

namespace cog2d {

enum CmdlineParamType : std::uint8_t
{
	CMDLINE_SWITCH,
	CMDLINE_STRING,
	CMDLINE_INT,
	CMDLINE_DOUBLE,

	CMDLINE_COUNT
};

struct CmdlineParam
{
	std::string name = "";
	char shortname = '\0';
	std::string description = "";

	void* value;
	CmdlineParamType type = CMDLINE_SWITCH;
};

struct CmdlineParams
{
	std::vector<CmdlineParam> options;
	std::vector<CmdlineParam> required;
	CmdlineParam variadic;
};

int cmdline_parse(int argc, char** argv, CmdlineParams params);
int cmdline_parse_arg(CmdlineParams& params, char*& arg, CmdlineParam*& out, bool& next);
int cmdline_parse_string(CmdlineParam& arg);

}  //namespace cog2d
