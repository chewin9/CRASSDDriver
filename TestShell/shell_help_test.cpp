#include <gmock/gmock.h>
#include "shell_help.h"
#include "test.h"
#include "process_executor.h"

class ShellHelpTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> mockExecutor;
	ShellHelp* shellHelp;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	void SetUp() override {
		shellHelp = new ShellHelp(&mockExecutor);

		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		delete shellHelp;
		std::cout.rdbuf(oldCoutStreamBuf);
	}
	void checkExpectedConsoleOutput(const std::string& expected) {
		std::string consoleData = oss.str();
		EXPECT_EQ(consoleData, expected);
	}
};
TEST_F(ShellHelpTestFixture, CrewIntroducePrintsCorrectOutput) {
	shellHelp->Run("help");
	std::string expected = "ÆÀ¸í: All - pass\n"
		"ÆÀ¿ø : ÀÌ½ÂÃ¶(ÆÀÀå), Áø¿¬±â, ¹ÚÁ¤±Ù, ÀÌÁØ¼®, ±è¿¹¼Ö, °­ÀÎÇý\n"
		"How to use help command\n"
		"  -help write\n"
		"  -help read\n"
		"  -help exit\n"
		"  -help fullwrite\n"
		"  -help fullread\n"
		"  -help erase\n"
		"  -help erase_range\n"
		"  -help flush\n";
	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelpwrite) {
	shellHelp->Run("help write");

	std::string expected = "write command: write [LBA] [Value]\n"
		"  - \"write\" must be in lowercase.\n"
		"  - [LBA]: 0 ~ 99\n"
		"  - [Value]:It must always start with 0x and be written as a 10-character string.\n"
		"  - ex) write 0 0x12345678\n";

	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelpread) {

	shellHelp->Run("help read");

	std::string expected = "read command: read [LBA]\n"
		"  - \"read\" must be in lowercase.\n"
		"  - [LBA]: 0 ~ 99\n"
		"  - ex) read 0 \n";

	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelpexit) {
	shellHelp->Run("help exit");

	std::string expected = "exit command: exit\n"
		"  - Test Shell is terminated.\n"
		"  - ex) exit\n";

	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelpfullwrite) {
	shellHelp->Run("help fullwrite");

	std::string expected = "fullwrite command: fullwrite [Value]\n"
		"  - A write operation is performed on all LBA areas.\n"
		"  - ex) fullwrite 0x12345678\n";

	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelpfullread) {
	shellHelp->Run("help fullread");

	std::string expected = "fullread command: fullread\n"
		"  - A read operation is performed from LBA 0 to 99.\n"
		"  - All values of the SSD are displayed on the screen.\n"
		"  - ex) fullread\n";

	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelperase) {
	shellHelp->Run("help erase");

	std::string expected =
		"erase command: erase [LBA] [SIZE]\n"
		"  - \"erase\" must be in lowercase.\n"
		"  - Delete content from a specific LBA to a specific SIZE.\n"
		"  - [LBA]: 0 ~ 99\n"
		"  - [SIZE]: SIZE can be negative, positive input\n"
		"            Delete down from LBA if negative\n"
		"            Delete up from LBA if positive\n"
		"  - ex) erase 10 30, erase 30 -10\n";

	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelperaserange) {
	shellHelp->Run("help erase_range");

	std::string expected =
		"erase_range command: erase [Start LBA] [End LBA]\n"
		"  - \"erase_range\" must be in lowercase.\n"
		"  - Delete content from a Start LBA to End LBA.\n"
		"  - Start LBA and end LBA are not in order.\n"
		"  - [Start LBA]: 0 ~ 99\n"
		"  - [End LBA]: 0 ~ 99\n"
		"  - ex) erase_range 10 20, erase_range 30 20\n";

	checkExpectedConsoleOutput(expected);
}

TEST_F(ShellHelpTestFixture, shellhelpflush) {
	shellHelp->Run("help flush");

	std::string expected =
		"flush command: flush\n"
		"  - \"flush\" must be in lowercase.\n"
		"  - Run all commands in the command buffer to empty the buffer.\n"
		"  - ex) flush\n";

	checkExpectedConsoleOutput(expected);
}