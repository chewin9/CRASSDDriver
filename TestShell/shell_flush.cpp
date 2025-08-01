#pragma once
#include "shell_flush.h"
#include "shell_util.h"
#include <string>
#include <vector>

TEST(FlushTest, checkSuccess) {
	MockProcessExecutor executor;
	ShellFlush flush(&executor);
	EXPECT_CALL(executor, Process("SSDDriver.exe F")).Times(1);

	flush.Run("flush");
}

TEST(FlushTest, checkExtraParamFailCase) {
	MockProcessExecutor executor;
	ShellFlush flush(&executor);

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
	std::cout.rdbuf(oss.rdbuf());

	EXPECT_EQ(false, flush.Run("flush ADDPARAMETER"));

	std::string originalStr = oss.str();
	EXPECT_EQ(originalStr, "INVALID PARAMETER\n");
	std::cout.rdbuf(oldCoutStreamBuf);
}

bool ShellFlush::Run(const std::string& cmd) {

	if (false == check_parameter_valid(cmd)) {
		Util::printInvalidCommand();
		return false;
	}
	executor_->Process("SSDDriver.exe F");
	return true;
}


bool ShellFlush::check_parameter_valid(const std::string& cmd) {
	std::vector<std::string> cmdLineVector = Util::splitBySpace(cmd);

	if (cmdLineVector.size() != 1) {
		return false;
	}
	if (cmdLineVector[0] != "flush") {
		return false;
	}
	return true;
}