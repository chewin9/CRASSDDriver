#pragma once
#include "iprocess_executor.h"
#include <string>
#include <vector>

class ShellFullWrite {
public:
	ShellFullWrite(IProcessExecutor* executor) : executor_(executor) {}

	void IssueFullWrite(const std::string& cmd);
	bool checkParameterValid(const std::string& cmd);
private:
	const int START_LBA = 0;
	const int END_LBA = 100;

	IProcessExecutor* executor_;

	std::vector<std::string> splitBySpace(const std::string& str);
	bool is_all_uppercase(const std::string& str);
	bool is_valid_unsigned(const std::string& str);
};