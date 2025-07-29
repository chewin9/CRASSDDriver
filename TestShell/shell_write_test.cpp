#include "gmock/gmock.h"
#include "iprocess_executor.h"
#include "shell_write.h"
#include "test.h"

#include <memory>
#include <string>
#include <cctype>

class ShellWriteTestFixture : public ::testing::Test {
public:
	testing::NiceMock<MockProcessExecutor> executor;
	ShellWrite* shell_write;

	void SetUp() override {
		shell_write = new ShellWrite(&executor);
	}

	void TearDown() override {
		delete shell_write;
	}
};

TEST_F(ShellWriteTestFixture, ssd_write) {

	EXPECT_CALL(executor, Process).Times(1).WillOnce(testing::Return(0));

	shell_write->IssueWrite("write 3 0xAAAABBBB");
}

TEST_F(ShellWriteTestFixture, ssd_write_checkparam_writeLBA) {
	//setup 
	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;
	oldCoutStreamBuf = std::cout.rdbuf();
	std::cout.rdbuf(oss.rdbuf());

	shell_write->IssueWrite("write 100000 0xAAAABBBB");

	std::string originalStr = oss.str();
	std::string INVALID_COMMAND = "INVALID_COMMAND\n";
	EXPECT_EQ(originalStr, INVALID_COMMAND);

	//teardown 
	std::cout.rdbuf(oldCoutStreamBuf);
}
