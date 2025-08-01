#include "gmock/gmock.h"
#include "shell_read.h"
#include "process_executor.h"
#include <fstream>
#include "test.h"

class ShellReadTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> mockExecutor;
	ShellRead* readShell;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	void SetUp() override {
		readShell = new ShellRead(&mockExecutor);

		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		delete readShell;
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

TEST_F(ShellReadTestFixture, TSRead01) {
	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "0x12345678";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

	readShell->Run("read 0");
	checkExpectedConsoleOutput("[Read] LBA 00 0x12345678\n");
	
}

TEST_F(ShellReadTestFixture, TSReadInvalidLBA01) {	//
	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "ERROR\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

	readShell->Run("read 101");
	checkExpectedConsoleOutput("ERROR\n");
}

TEST_F(ShellReadTestFixture, TSReadInvalidLBA02) {	//
	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "ERROR\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

	readShell->Run("read -1");
	checkExpectedConsoleOutput("ERROR\n");
}

TEST_F(ShellReadTestFixture, TSReadInvalidLBA03) {	//
	readShell->Run("read");
	checkExpectedConsoleOutput("INVALID PARAMETER\n");
}

TEST_F(ShellReadTestFixture, TSReadInvalidLBA04) {	//
	readShell->Run("read 0x4");
	checkExpectedConsoleOutput("INVALID PARAMETER\n");
}
