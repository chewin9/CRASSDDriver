#include "shell_erase_range.h"
#include "shell_util.h"
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

ShellEraseRange::ShellEraseRange(IProcessExecutor* executor) : executor_(executor) {}

bool ShellEraseRange::Run(const std::string& input) {
	std::vector<std::string> commandVector = Util::splitBySpace(input);

	if (checkParameterValid(commandVector) == false) {
		Util::printInvalidParameter();
		return false;
	}
	int startLba = getStartLba(std::stoi(commandVector.at(1)), std::stoi(commandVector.at(2)));
	int curSize = getEraseSize(std::stoi(commandVector.at(1)), std::stoi(commandVector.at(2)));

	calculateRangeAndPerformSSD(startLba, curSize);
	return true;
}

bool ShellEraseRange::checkParameterValid(std::vector<std::string> commandVec) {
	if (commandVec.size() != VALID_ERASE_RANGE_COMMAND_SIZE) return false;

	if (isDecimalString(commandVec.at(START_LBA)) == false ||
		isDecimalString(commandVec.at(END_LBA)) == false) {
		return false;
	}
	int startLba, endLba;

	try {
		startLba = std::stoi(commandVec.at(START_LBA));
		endLba = std::stoi(commandVec.at(END_LBA));
	}
	catch (const std::invalid_argument&) {
		return false; // 숫자가 아님
	}
	catch (const std::out_of_range&) {
		return false; // 범위를 벗어남
	}
	if (startLba < MIN_INDEX || startLba >= MAX_INDEX ||
		endLba < MIN_INDEX || endLba >= MAX_INDEX) {
		return false;
	}
	return true;
}

int ShellEraseRange::getStartLba(int num1, int num2) {
	return std::min(num1, num2);
}

int ShellEraseRange::getEraseSize(int num1, int num2) {
	return std::abs(num1 - num2) + 1;
}

void ShellEraseRange::performEraseToSSD(int index, int size) {
	std::string cmdLine;
	cmdLine = "SSDDriver.exe E " + std::to_string(index) + " " + std::to_string(size);
	executor_->Process(cmdLine);
#if(CONSOLE_TEST)
	std::string log = "(" + std::to_string(index) + ", " + std::to_string(size) + ")\n";
	Logger::getInstance().print(__FUNCTION__, log);
	std::cout << "(" << index << ", " << size << ")\n";
#endif
}