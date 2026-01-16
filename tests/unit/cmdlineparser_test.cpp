#include <gtest/gtest.h>

#include <cog2d/util/cmdlineparser.hpp>

#include <cstring>

using namespace cog2d;

TEST(CmdlineParserTest, NoneTest)
{
	bool result = false;
	CmdlineParams args = {{{"test", 't', "This is a test", &result, CMDLINE_SWITCH}}};
	char* argv[] = {"./game"};
	int resp;

	resp = cmdline_parse(1, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_FALSE(result);
}

TEST(CmdlineParserTest, SwitchTest)
{
	bool result = false;
	CmdlineParams args = {
	    {
	        {"test", 't', "This is a test", &result, CMDLINE_SWITCH},
	    },
	    {{"test2", '\0', "aeiou", nullptr, CMDLINE_STRING}},
	};
	char* argv[5] = {"./game", "--test"};
	int resp;

	resp = cmdline_parse(2, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(result);

	result = false;
	argv[1] = "-t";
	resp = cmdline_parse(2, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(result);

	result = false;
	argv[1] = "--test";
	argv[2] = "hello";
	resp = cmdline_parse(2, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(result);

	result = false;
	argv[1] = "-t";
	argv[2] = "hello";
	resp = cmdline_parse(2, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(result);
}

TEST(CmdlineParserTest, StringTest)
{
	std::string result = "";
	CmdlineParams args = {
	    {
	        {"test", 't', "This is a test", &result, CMDLINE_STRING},
	    },
	};
	char* argv[5] = {"./game", "--test", "value"};
	int resp;

	resp = cmdline_parse(3, (char**) argv, args);
	EXPECT_GE(resp, 0);
	EXPECT_STREQ(result.c_str(), argv[2]);
}
