#include "gmock/gmock.h"
#include "shell_full_read.h"
#include "process_executor.h"
#include <fstream>
#include "test.h"

class ShellFullReadTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> mockExecutor;
	ShellFullRead* shellFullRead;
	

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	void SetUp() override {
		shellFullRead = new ShellFullRead(&mockExecutor);

		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		delete shellFullRead;
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

TEST_F(ShellFullReadTestFixture, TSFullRead01) {	//
	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "0xABCDEEEE\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			})
		.WillRepeatedly([](const std::string& cmd) {
				// 파일 생성
				std::ofstream file("ssd_output.txt");
				file << "0x12345678\n";
				file.close();
				// int 반환 (mock이므로 임의 값 반환)
				return 0;
			});

			shellFullRead->Run("fullread");
			checkExpectedConsoleOutputPart("0 0xABCDEEEE");
}

TEST_F(ShellFullReadTestFixture, TSFullRead02) {	//
	shellFullRead->Run("fullread 1");
	checkExpectedConsoleOutput("INVALID COMMAND\n");
}
