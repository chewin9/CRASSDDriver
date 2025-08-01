#pragma once
#include "gmock/gmock.h"
#include "test.h"
#include "ishell_command.h"

class ShellFlush : public IShellCommand {
public:
	ShellFlush(IProcessExecutor* executor) : executor_(executor) {}
	bool Run(const std::string& cmd) override;
private:
	std::vector<std::string> split_by_space(const std::string& str);
	bool check_parameter_valid(const std::string& cmd);
	void print_invalid_command(void);
	IProcessExecutor* executor_;
	const std::string INVALID_PARAMETER = "INVALID PARAMETER";

};