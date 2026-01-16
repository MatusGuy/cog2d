#include "cmdlineparser.hpp"

#include <cstring>

namespace cog2d {
int cmdline_parse(int argc, char** argv, CmdlineParams params)
{
	char* arg;
	std::size_t arglen;
	CmdlineParam* parsedparam = nullptr;
	for (int i = 1; i < argc; ++i) {
		arg = argv[i];
		arglen = std::strlen(arg);

		if (!parsedparam)
			parsedparam = cmdline_parse_arg(arg, params);

		if (!parsedparam)
			// Invalid argument
			return -1;

		switch (parsedparam->type) {
		case CMDLINE_SWITCH:
			(*parsedparam->value.b) = true;
			break;

		default:
			break;
		}

		parsedparam = nullptr;
	}

	return 0;
}

CmdlineParam* cmdline_parse_arg(char*& arg, CmdlineParams& params)
{
	CmdlineParam* out;
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
		} else {
			// Short-form options
			for (CmdlineParam& a : params.options) {
				if (arg[1] == a.shortname) {
					out = &a;
				}
			}

			if (!out)
				return out;

			if (out->type != CMDLINE_SWITCH && arglen >= 3) {
				// With 'o' as the shortname of some option, get the value from what
				// comes after:
				//
				//   -osomething
				//     ^^^^^^^^^
				//
				// If this is not the case for this argument (in other words, arglen < 3), then get
				// the value from the next argument
				arg = &arg[2];
			}
		}

		return out;
	}

	// Parameters

	// Variadic arguments

	return out;
}

}  //namespace cog2d
