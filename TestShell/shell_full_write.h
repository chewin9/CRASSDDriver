#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include "shell_write.h"
#include <string>
#include <vector>

class ShellFullWrite : public Write , public IShellCommand{
public:
	ShellFullWrite(IProcessExecutor* executor) : executor_(executor) {}

	bool Run(const std::string& cmd) override;
	bool checkParameterValid(const std::string& cmd);
private:
	IProcessExecutor* executor_;
};