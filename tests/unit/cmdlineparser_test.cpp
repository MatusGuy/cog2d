#include <gtest/gtest.h>

#include <cog2d/util/cmdlineparser.hpp>

#include <cstring>

using namespace cog2d;

TEST(CmdlineParserTest, NoneTest)
{
	bool result = false;
	CmdlineParams args = {{{"test", 't', "This is a test", &result, CMDLINE_SWITCH}}};
	char* argv[] = {"./game"};

	int resp = cmdline_parse(sizeof(argv) / sizeof(char*), (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_FALSE(result);
}

TEST(CmdlineParserTest, SwitchTest)
{
	bool result = false;
	CmdlineParams args = {{{"test", 't', "This is a test", &result, CMDLINE_SWITCH}}};
	char* argv[] = {"./game", "--test"};
	int resp;

	resp = cmdline_parse(sizeof(argv) / sizeof(char*), (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(result);

	result = false;
	argv[1] = "-t";
	resp = cmdline_parse(sizeof(argv) / sizeof(char*), (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(result);
}
