#include "gmock/gmock.h"
#include "shell_erase.h"
#include "process_executor.h"
#include <fstream>
#include "test.h"

class ShellEraseTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> mockExecutor;
	ShellErase* shellErase;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	void SetUp() override {
		shellErase = new ShellErase(&mockExecutor);

		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		delete shellErase;
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


TEST_F(ShellEraseTestFixture, TSErase01) {
	shellErase->Run("erase 5 10");
	checkExpectedConsoleOutput("(5, 10)\n");
}

TEST_F(ShellEraseTestFixture, TSErase02) {
	shellErase->Run("erase 5 20");
	checkExpectedConsoleOutput("(5, 10)\n(15, 10)\n");
}

TEST_F(ShellEraseTestFixture, TSErase03) {
	shellErase->Run("erase 5 26");
	checkExpectedConsoleOutput("(5, 10)\n(15, 10)\n(25, 6)\n");
}
TEST_F(ShellEraseTestFixture, TSErase04) {
	shellErase->Run("erase 30 -25");
	checkExpectedConsoleOutput("(21, 10)\n(11, 10)\n(6, 5)\n");
}
TEST_F(ShellEraseTestFixture, TSErase05) {
	shellErase->Run("erase 30 -50");
	checkExpectedConsoleOutput("(21, 10)\n(11, 10)\n(1, 10)\n(0, 1)\n");
}

TEST_F(ShellEraseTestFixture, TSErase06) {
	shellErase->Run("erase 80 1000");
	checkExpectedConsoleOutput("(80, 10)\n(90, 10)\n");
}

TEST_F(ShellEraseTestFixture, TSInvalidErase01) {
	shellErase->Run("erase 80");
	checkExpectedConsoleOutput("INVALID PARAMETER\n");
}

TEST_F(ShellEraseTestFixture, TSInvalidErase02) {
	shellErase->Run("erase -1 199");
	checkExpectedConsoleOutput("INVALID PARAMETER\n");
}
TEST_F(ShellEraseTestFixture, TSInvalidErase03) {
	shellErase->Run("erase AA 123");
	checkExpectedConsoleOutput("INVALID PARAMETER\n");
}
TEST_F(ShellEraseTestFixture, TSInvalidErase04) {
	shellErase->Run("erase 0x1 123");
	checkExpectedConsoleOutput("INVALID PARAMETER\n");
}
TEST_F(ShellEraseTestFixture, TSInvalidErase05) {
	shellErase->Run("erase 0 0xA");
	checkExpectedConsoleOutput("INVALID PARAMETER\n");
}
