#include "gmock/gmock.h"
#include "shell_read.h"
#include "process_executor.h"
#include <fstream>
#include "test.h"

class ShellReadTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> mockExecutor;
	ShellRead* readShell;

	void SetUp() override {
		readShell = new ShellRead(&mockExecutor);
	}

	void TearDown() override {
		delete readShell;
		if (std::remove("ssd_output.txt") == 0) {
		}
	}
};

TEST_F(ShellReadTestFixture, TSRead01) {
	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "0 0x12345678\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

	EXPECT_EQ(readShell->read("read 0"), "0x12345678");
}

TEST_F(ShellReadTestFixture, TSRead02) {
	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "0 0x12345678\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

	EXPECT_EQ(readShell->read("read 99"), "0x00000000");

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

	EXPECT_EQ(readShell->read("read 101"), "ERROR");
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

	EXPECT_EQ(readShell->read("read -1"), "ERROR");
}

TEST_F(ShellReadTestFixture, TSFullRead01) {	//
	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "2 0xABCDEEEE\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			})
		.WillRepeatedly([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "0 0x12345678\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

		EXPECT_THAT(readShell->fullRead("fullread"), 
			::testing::AllOf(
				::testing::HasSubstr("2 0xABCDEEEE"),
				::testing::HasSubstr("0 0x12345678")
			)
		);
		
}

TEST_F(ShellReadTestFixture, TSFullRead02) {	//
	EXPECT_CALL(mockExecutor, Process)
		.WillRepeatedly([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "0 0x12345678\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

	EXPECT_THAT(readShell->fullRead("fullread"), ::testing::HasSubstr("0 0x12345678"));
}
