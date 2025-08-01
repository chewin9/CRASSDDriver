#pragma once
#include "gmock/gmock.h"
#include "test.h"
#include "ishell_command.h"

class ShellFlush : public IShellCommand {
public:
	ShellFlush(IProcessExecutor* executor) : executor_(executor) {}
	bool Run(const std::string& cmd) override;
private:
	bool check_parameter_valid(const std::string& cmd);
	IProcessExecutor* executor_;
};