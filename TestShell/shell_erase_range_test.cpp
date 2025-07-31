#include "gmock/gmock.h"
#include "shell_erase_range.h"
#include "process_executor.h"
#include <fstream>
#include "test.h"

class ShellEraseRangeTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> mockExecutor;
	ShellEraseRange* shellEraseRange;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	void SetUp() override {
		shellEraseRange = new ShellEraseRange(&mockExecutor);

		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		delete shellEraseRange;
		std::cout.rdbuf(oldCoutStreamBuf);
		std::remove("ssd_output.txt");
	}

	void checkExpectedConsoleOutput(const std::string& expected) {
		std::string consoleData = oss.str();
		EXPECT_EQ(consoleData, expected);
	}

	void checkExpectedConsoleOutputPart(const std::string& expected) {
		std::string consoleData = oss.str();
		EXPECT_THAT(consoleData, ::testing::HasSubstr(expected));
	}

};


#if (CONSOLE_TEST)
TEST_F(ShellEraseRangeTestFixture, TSEraseRange01) {
	shellEraseRange->Run("erase 5 10");
	checkExpectedConsoleOutput("(5, 6)\n");
}
TEST_F(ShellEraseRangeTestFixture, TSEraseRange02) {
	shellEraseRange->Run("erase 5 99");
	checkExpectedConsoleOutput("(5, 10)\n(15, 10)\n(25, 10)\n(35, 10)\n(45, 10)\n(55, 10)\n(65, 10)\n(75, 10)\n(85, 10)\n(95, 5)\n");
}

TEST_F(ShellEraseRangeTestFixture, TSEraseRange03) {
	shellEraseRange->Run("erase 99 10");
	checkExpectedConsoleOutput("(10, 10)\n(20, 10)\n(30, 10)\n(40, 10)\n(50, 10)\n(60, 10)\n(70, 10)\n(80, 10)\n(90, 10)\n");
}

TEST_F(ShellEraseRangeTestFixture, TSEraseRange04) {
	shellEraseRange->Run("erase 10 10");
	checkExpectedConsoleOutput("(10, 1)\n");
}

TEST_F(ShellEraseRangeTestFixture, TSEraseRange05) {
	shellEraseRange->Run("erase 0 99");
	checkExpectedConsoleOutput("(0, 10)\n(10, 10)\n(20, 10)\n(30, 10)\n(40, 10)\n(50, 10)\n(60, 10)\n(70, 10)\n(80, 10)\n(90, 10)\n");
}

TEST_F(ShellEraseRangeTestFixture, TSEraseRangeInvalid01) {
	shellEraseRange->Run("erase 0 -1");
	checkExpectedConsoleOutput("INVALID COMMAND\n");
}

TEST_F(ShellEraseRangeTestFixture, TSEraseRangeInvalid02) {
	shellEraseRange->Run("erase 0 100");
	checkExpectedConsoleOutput("INVALID COMMAND\n");
}

TEST_F(ShellEraseRangeTestFixture, TSEraseRangeInvalid03) {
	shellEraseRange->Run("erase 0");
	checkExpectedConsoleOutput("INVALID COMMAND\n");
}

#endif