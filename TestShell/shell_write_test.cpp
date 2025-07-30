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
	ShellWrite* shell_write;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	const std::string INVALID_COMMAND = "INVALID_COMMAND\n";
	const std::string WRITE_DONE = "[Write] Done\n";

	void SetUp() override {
		shell_write = new ShellWrite(&executor);

		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		std::cout.rdbuf(oldCoutStreamBuf);

		delete shell_write;
	}
};

TEST_F(ShellWriteTestFixture, ssd_write) {
	EXPECT_CALL(executor, Process).Times(1).WillOnce(testing::Return(0));
	shell_write->IssueWrite("write 3 0xAAAABBBB");
	std::string originalStr = oss.str();
	EXPECT_EQ(originalStr, WRITE_DONE);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_Read) {
	shell_write->IssueWrite("read 3 0xAAAABBBB");
	std::string originalStr = oss.str();
	EXPECT_EQ(originalStr, INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_writeLBA) {

	shell_write->IssueWrite("write 100000 0xAAAABBBB");
	std::string originalStr = oss.str();
	EXPECT_EQ(originalStr, INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_invalid_data) {

	shell_write->IssueWrite("write 22 ZXCVBNMAqp");
	std::string originalStr = oss.str();
	EXPECT_EQ(originalStr, INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_invalid_data2) {

	shell_write->IssueWrite("write 22 0xZXCVBNMA");
	std::string originalStr = oss.str();
	EXPECT_EQ(originalStr, INVALID_COMMAND);
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_invalid_data3) {

	shell_write->IssueWrite("write aa 0xZXCVBNMA");
	std::string originalStr = oss.str();
	EXPECT_EQ(originalStr, INVALID_COMMAND);
}

TEST(TestFullWrite, Success) {
	testing::NiceMock<MockProcessExecutor> executor;
	ShellFullWrite* full_write = new ShellFullWrite(&executor);

	EXPECT_CALL(executor, Process).Times(100).WillRepeatedly(testing::Return(0));

	// expect all LBA write
	full_write->IssueFullWrite("fullwrite 0xABCDFFFF");
}