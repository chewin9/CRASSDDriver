#include "gmock/gmock.h"
#include "iprocess_executor.h"
#include "shell_write.h"
#include "shell_full_write.h"
#include "test.h"

#include <memory>
#include <string>
#include <cctype>

class ShellWriteTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> executor;
	ShellFullWrite* full_write;
	ShellWrite* shell_write;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	const std::string INVALID_COMMAND = "INVALID PARAMETER\n";
	const std::string WRITE_DONE = "[Write] Done\n";
	const std::string WRITE_FULL_DONE = "[WriteFull] Done\n";

	void SetUp() override {
		shell_write = new ShellWrite(&executor);
		full_write = new ShellFullWrite(&executor);
		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		std::cout.rdbuf(oldCoutStreamBuf);

		delete shell_write;
	}

	void checkExpectedConsoleOutput(const string& str) {
		std::string originalStr = oss.str();
		EXPECT_EQ(originalStr, str);
	}
};

TEST_F(ShellWriteTestFixture, ssd_write) {
	EXPECT_CALL(executor, Process).Times(1).WillOnce(testing::Return(0));
	shell_write->Run("write 3 0xAAAABBBB");
	checkExpectedConsoleOutput(WRITE_DONE);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_Read) {
	shell_write->Run("read 3 0xAAAABBBB");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_writeLBA) {
	shell_write->Run("write 100000 0xAAAABBBB");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_invalid_data) {
	shell_write->Run("write 22 ZXCVBNMAqp");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_invalid_data2) {
	shell_write->Run("write 22 0xZXCVBNMA");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_invalid_data3) {
	shell_write->Run("write aa 0xZXCVBNMA");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, full_write_success) {
	// expect all LBA write
	EXPECT_CALL(executor, Process).Times(100).WillRepeatedly(testing::Return(0));

	full_write->Run("fullwrite 0xABCDFFFF");
	checkExpectedConsoleOutput(WRITE_FULL_DONE);
}

TEST_F(ShellWriteTestFixture, full_write_invalid_param_overHexValue) {
	full_write->Run("fullwrite 0xQWERTYUI");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, full_write_invalid_param_notHexFormat) {
	full_write->Run("fullwrite 12345678");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, full_write_invalid_param_multiParam) {
	full_write->Run("fullwrite 99 0xABCDFFFF");
	checkExpectedConsoleOutput(INVALID_COMMAND);
}