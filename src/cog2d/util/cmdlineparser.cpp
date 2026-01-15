#include "cmdlineparser.hpp"

#include <cstring>

namespace cog2d {
int cmdline_parse(const int argc, char** const argv, const CmdlineArgs args)
{
	char* arg;
	CmdlineArg const* parsedarg = nullptr;
	for (int i = 1; i < argc; ++i) {
		arg = argv[i];

		if (std::strlen(arg) >= 2 && arg[0] == '-') {
			if (std::strlen(arg) >= 3 && arg[1] == '-') {
				// Long-form options
				arg += 2;
				for (CmdlineArg const& a : args.options) {
					if (std::strcmp(arg, a.name.c_str()) == 0) {
						parsedarg = &a;
					}
				}
			} else {
				// Short-form options
			}

		} else {
			// Required arguments and variadic arguments
		}

		if (!parsedarg)
			continue;

		switch (parsedarg->type) {
		case CMDLINE_SWITCH:
			(*parsedarg->value.b) = true;
			break;

		default:
			break;
		}
	}

	return 0;
}

}  //namespace cog2d
