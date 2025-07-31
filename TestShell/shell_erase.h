#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include <string>
#include <vector>

#define CONSOLE_TEST true

class Erase {
public:
	const int MIN_INDEX = 0;
	const int MAX_INDEX = 100;
	const std::string INVALID_COMMAND = "INVALID COMMAND";
	void printInvalidCommand();
	void calculateRangeAndPerformSSD(int start, int size);
	
	std::vector<std::string> splitBySpace(const std::string& cmd);

	virtual void performEraseToSSD(int index, int size) = 0;
};

class ShellErase : public Erase, public IShellCommand {
public:
	ShellErase(IProcessExecutor* executor);
	bool Run(const std::string& input) override;
	void performEraseToSSD(int index, int size) override;
private:
	IProcessExecutor* executor_;
	bool checkParameterValid(std::vector<std::string> commandVec);
	
};