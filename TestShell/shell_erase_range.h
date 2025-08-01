#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include "shell_erase.h"
#include <string>
#include <vector>

class ShellEraseRange : public Erase, public IShellCommand {
public:
	ShellEraseRange(IProcessExecutor* executor);
	bool Run(const std::string& input) override;
	void performEraseToSSD(int index, int size) override;
private:
	IProcessExecutor* executor_;
	bool checkParameterValid(std::vector<std::string> commandVec);
	int getStartLba(int num1, int num2);
	int getEraseSize(int num1, int num2);
};