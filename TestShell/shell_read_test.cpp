#include "gmock/gmock.h"
#include "shell.h"

TEST(TS, TSRead01) {
	Shell sh;
	std::string str;
	std::string actual = sh.read(str);

	EXPECT_EQ("0x12345678", actual);
}