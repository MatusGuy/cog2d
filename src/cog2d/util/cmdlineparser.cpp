#include "cmdlineparser.hpp"

#include <cstring>

namespace cog2d {
int cmdline_parse(const int argc, char** const argv, const CmdlineArgs args)
{
	char* arg;
	std::size_t arglen;
	CmdlineArg const* parsedarg = nullptr;
	for (int i = 1; i < argc; ++i) {
		arg = argv[i];
		arglen = std::strlen(arg);

		if (!parsedarg) {
			if (arglen >= 2 && arg[0] == '-') {
				if (arglen >= 3 && arg[1] == '-') {
					// Long-form options
					arg = &arg[2];
					for (CmdlineArg const& a : args.options) {
						if (std::strcmp(arg, a.name.c_str()) == 0) {
							parsedarg = &a;
						}
					}
				} else {
					// Short-form options
					for (CmdlineArg const& a : args.options) {
						if (arg[1] == a.shortname) {
							parsedarg = &a;
						}
					}

					if (parsedarg->type != CMDLINE_SWITCH) {
						if (arglen >= 3) {
							// With 'o' as the shortname of some option, get the value from what
							// comes after:
							//
							//   -osomething
							//     ^^^^^^^^^
							arg = &arg[2];
						} else {
							// Get the value from the next argument
							continue;
						}
					}
				}

			} else {
				// Required arguments and variadic arguments
			}
		}

		if (!parsedarg)
			// Invalid argument
			return -1;

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
