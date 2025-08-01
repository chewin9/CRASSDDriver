#pragma once
#include "shell_flush.h"
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
	EXPECT_EQ(originalStr, "INVALID COMMAND\n");
	std::cout.rdbuf(oldCoutStreamBuf);
}

bool ShellFlush::Run(const std::string& cmd) {

	if (false == check_parameter_valid(cmd)) {
		print_invalid_command();
		return false;
	}
	executor_->Process("SSDDriver.exe F");
	return true;
}

std::vector<std::string> ShellFlush::split_by_space(const std::string& str) {
	std::istringstream iss(str);
	std::vector<std::string> tokens;
	std::string word;
	while (iss >> word) {
		tokens.push_back(word);
	}
	return tokens;
}

bool ShellFlush::check_parameter_valid(const std::string& cmd) {
	std::vector<std::string> cmdLineVector = split_by_space(cmd);

	if (cmdLineVector.size() != 1) {
		return false;
	}
	if (cmdLineVector[0] != "flush") {
		return false;
	}
	return true;
}

void ShellFlush::print_invalid_command(void) {
	std::cout << "INVALID COMMAND" << std::endl;
}
