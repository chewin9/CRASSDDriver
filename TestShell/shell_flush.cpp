#pragma once
#include "shell_flush.h"

TEST(FlushTest, TC1) {
	MockProcessExecutor executor;
	ShellFlush flush(&executor);
	EXPECT_CALL(executor, Process("F")).Times(1);

	flush.Run("flush");
}

TEST(FlushTest, TC2) {
	MockProcessExecutor executor;
	ShellFlush flush(&executor);

	EXPECT_EQ(false, flush.Run("flush ADDPARAMETER"));
}


bool ShellFlush::Run(const std::string& cmd) {
	
	// check parameter valid 
	if (cmd != "flush") {
		std::cout << "INVALID COMMAND" << std::endl;
		return false;
	}

	executor_->Process("F");
	return true;
}