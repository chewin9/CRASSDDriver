#pragma once
#include "iprocess_executor.h"

class ProcessExecutor : public IProcessExecutor {
public:
	int Process(const std::string& commandLine) override {
		// 실제 사용: system(), CreateProcess(), etc
		return std::system(commandLine.c_str());
	}
};