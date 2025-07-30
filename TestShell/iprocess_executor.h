#pragma once
#include <string>

class IProcessExecutor {
public:
	virtual ~IProcessExecutor() = default;
	virtual int Process(const std::string& commandLine) = 0;
};

