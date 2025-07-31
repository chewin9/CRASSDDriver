#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include "shell_read.h"
#include <string>
#include <tuple>

class ShellFullRead : public Read , public IShellCommand {
public:
	ShellFullRead(IProcessExecutor* executor);
	bool Run(const std::string& input) override;
	virtual void performReadToSSD(std::string index) override;

private:
	IProcessExecutor* executor_;
	bool checkParameterValid(std::vector<std::string> commandVec);
	std::string appendSsdData(int index, const std::string& Value);
};