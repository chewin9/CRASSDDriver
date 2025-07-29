#pragma once
#include "iprocess_executor.h"

class ProcessExecutor : public IProcessExecutor {
public:
	int Process(const std::string& commandLine) override {
		// ���� ���: system(), CreateProcess(), etc
		return std::system(commandLine.c_str());
	}
};