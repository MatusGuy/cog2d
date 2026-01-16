#include "cmdlineparser.hpp"

#include <cstring>

namespace cog2d {
int cmdline_parse(int argc, char** argv, CmdlineParams params)
{
	bool skip;
	char* arg;
	CmdlineParam* parsedparam = nullptr;

	for (int i = 1; i < argc; ++i) {
		skip = false;
		arg = argv[i];

		if (!parsedparam && cmdline_parse_arg(params, arg, parsedparam, skip) < 0) {
			// Invalid argument
			return -1;
		}

		if (skip)
			continue;

		switch (parsedparam->type) {
		case CMDLINE_SWITCH:
			*((bool*) parsedparam->value) = true;
			break;

		case CMDLINE_STRING:
			*((std::string*) parsedparam->value) = std::string(arg);
			break;

		case CMDLINE_INT:
			for (int j = 0; j < std::strlen(arg); ++j) {
				if (arg[j] == '.')
					// Invalid int
					return -2;
			}

			*((int*) parsedparam->value) = std::stoi(arg);
			break;

		case CMDLINE_DOUBLE:
			*((double*) parsedparam->value) = std::stod(arg);
			break;

		default:
			break;
		}

		parsedparam = nullptr;
	}

	return 0;
}

int cmdline_parse_arg(CmdlineParams& params, char*& arg, CmdlineParam*& out, bool& next)
{
	next = true;

	std::size_t arglen = std::strlen(arg);

	if (arglen >= 2 && arg[0] == '-') {
		if (arglen >= 3 && arg[1] == '-') {
			// Long-form options
			arg = &arg[2];
			for (CmdlineParam& a : params.options) {
				if (std::strcmp(arg, a.name.c_str()) == 0) {
					out = &a;
				}
			}

			if (!out)
				return -1;
		} else {
			// Short-form options
			for (CmdlineParam& a : params.options) {
				if (arg[1] == a.shortname) {
					out = &a;
				}
			}

			if (!out)
				return -1;

			if (out->type != CMDLINE_SWITCH) {
				if (arglen >= 3) {
					// With 'o' as the shortname of some option, get the value from what
					// comes after:
					//
					//   -osomething
					//     ^^^^^^^^^
					arg = &arg[2];
					next = false;
				}
			}
		}

		if (out->type == CMDLINE_SWITCH)
			next = false;

		return 0;
	}

	// Parameters

	// Variadic arguments

	return 0;
}

}  //namespace cog2d
