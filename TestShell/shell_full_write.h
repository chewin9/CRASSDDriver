#pragma once
#include "iprocess_executor.h"
#include <string>
#include <vector>

class ShellFullWrite {
public:
	ShellFullWrite(IProcessExecutor* executor) : executor_(executor) {}

	void IssueFullWrite(std::string cmd);
private:
	IProcessExecutor* executor_;

	std::vector<std::string> splitBySpace(const std::string& str);
	bool is_valid_unsigned(const std::string& str);
};