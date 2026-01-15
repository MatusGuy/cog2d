#include <gtest/gtest.h>

#include <cog2d/util/cmdlineparser.hpp>

#include <cstring>

using namespace cog2d;

TEST(CmdlineParserTest, SwitchTest)
{
	bool result = false;
	CmdlineArgs args = {{{"test", 't', "This is a test", &result, CMDLINE_SWITCH}}};
	char* argv[5] = {"./game"};

	int resp = cmdline_parse(1, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_FALSE(result);

	argv[1] = "--test";
	resp = cmdline_parse(2, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(result);

	/*
	result = false;
	int s_resp = cmdline_parse(2, { "./game", "-t" }, args);
	*/
}
