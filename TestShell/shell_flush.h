#pragma once
#include "gmock/gmock.h"
#include "test.h"
#include "ishell_command.h"

class ShellFlush : public IShellCommand {
public:
	ShellFlush(IProcessExecutor* executor) : executor_(executor) {}
	bool Run(const std::string& cmd) override;
private:
	IProcessExecutor* executor_;
};