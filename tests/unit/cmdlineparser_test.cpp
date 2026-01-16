#include <gtest/gtest.h>

#include <cog2d/util/cmdlineparser.hpp>

#include <cstring>

using namespace cog2d;

bool v_switch;
std::string v_string;
int v_int;
double v_double;
CmdlineParams PARAMS = {
    {
        {"switch", 'o', "This is a test switch", &v_switch, CMDLINE_SWITCH},
        {"string", 's', "This is a test string option", &v_string, CMDLINE_STRING},
        {"int", 'i', "This is a test int option", &v_int, CMDLINE_INT},
        {"double", 'd', "This is a test double option", &v_double, CMDLINE_DOUBLE},
    },
};

TEST(CmdlineParserTest, NoneTest)
{
	char* argv[] = {"./game"};
	int resp;

	resp = cmdline_parse(1, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_FALSE(v_switch);
}

TEST(CmdlineParserTest, SwitchTest)
{
	char* argv[5] = {"./game", "--switch"};
	int resp;

	resp = cmdline_parse(2, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(v_switch);

	v_switch = false;
	argv[1] = "-o";
	resp = cmdline_parse(2, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(v_switch);

	v_switch = false;
	argv[1] = "--switch";
	argv[2] = "hello";
	resp = cmdline_parse(2, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(v_switch);

	/*
	v_switch = false;
	argv[1] = "-t";
	argv[2] = "hello";
	resp = cmdline_parse(2, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(v_switch);
	*/
}

TEST(CmdlineParserTest, StringTest)
{
	char* argv[5] = {"./game", "--string", "value"};
	int resp;

	resp = cmdline_parse(3, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_STREQ(v_string.c_str(), argv[2]);

	argv[1] = "-s";
	resp = cmdline_parse(3, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_STREQ(v_string.c_str(), argv[2]);

	argv[1] = "-svalue";
	resp = cmdline_parse(2, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_STREQ(v_string.c_str(), "value");
}

TEST(CmdlineParserTest, IntTest)
{
	char* argv[5] = {"./game", "--int", "10"};
	int resp;

	resp = cmdline_parse(3, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_EQ(v_int, 10);

	argv[2] = "1.5";
	v_int = 0;
	resp = cmdline_parse(3, (char**) argv, PARAMS);
	EXPECT_EQ(v_int, 0);
	EXPECT_EQ(resp, -2);
	argv[2] = "10";

	argv[1] = "-i";
	resp = cmdline_parse(3, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_EQ(v_int, 10);

	argv[1] = "-i10";
	resp = cmdline_parse(2, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_EQ(v_int, 10);
}

TEST(CmdlineParserTest, DoubleTest)
{
	char* argv[5] = {"./game", "--double", "2.5"};
	double resp;

	resp = cmdline_parse(3, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_DOUBLE_EQ(v_double, 2.5);

	argv[1] = "-d";
	resp = cmdline_parse(3, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_DOUBLE_EQ(v_double, 2.5);

	argv[1] = "-d2.5";
	resp = cmdline_parse(2, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_DOUBLE_EQ(v_double, 2.5);
}

TEST(CmdlineParserTest, AllTest)
{
	char* argv[10] = {"./game", "-o", "--string", "test", "-i1", "--double", "7.125"};
	double resp;

	resp = cmdline_parse(7, (char**) argv, PARAMS);
	EXPECT_GE(resp, 0);
	EXPECT_TRUE(v_switch);
	EXPECT_STREQ(v_string.c_str(), "test");
	EXPECT_EQ(v_int, 1);
	EXPECT_DOUBLE_EQ(v_double, 7.125);
}
