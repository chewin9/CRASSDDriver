#pragma once
#include "iprocess_executor.h"
#include <string>
#include <vector>
#include <tuple>
class ShellFullRead {
public:
	ShellFullRead(IProcessExecutor* executor);
	void fullread(const std::string input);

private:
	IProcessExecutor* executor_;

	const std::string INVALID_COMMAND = "INVALID COMMAND";
	const int MIN_INDEX = 0;
	const int MAX_INDEX = 100;

	void runSSDDriver(std::string index);
	std::tuple<std::string, std::string> parse_command(const std::string& input);
	std::string appendSSDData(void);
	std::string getSsdOutputData(void);
	void printInvalidCommand(void);
};