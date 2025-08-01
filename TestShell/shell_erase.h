#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include <string>
#include <vector>

#define CONSOLE_TEST false

class Erase {
public:
	const int MIN_INDEX = 0;
	const int MAX_INDEX = 100;
	const int MAX_SSD_ERASE_SIZE = 10;
	void calculateRangeAndPerformSSD(int start, int size);
	bool isDecimalString(const std::string& str);

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
	const int START_LBA_INDEX = 1;
	const int ERASE_SIZE_INDEX = 2;
	const int VALID_ERASE_COMMAND_SIZE = 3;
};