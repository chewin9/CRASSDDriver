#pragma once
#include "shell_flush.h"
#include <string>
#include <vector>

TEST(FlushTest, TC1) {
	MockProcessExecutor executor;
	ShellFlush flush(&executor);
	EXPECT_CALL(executor, Process("SSDDriver.exe F")).Times(1);

	flush.Run("flush");
}

TEST(FlushTest, TC2) {
	MockProcessExecutor executor;
	ShellFlush flush(&executor);

	EXPECT_EQ(false, flush.Run("flush ADDPARAMETER"));
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
