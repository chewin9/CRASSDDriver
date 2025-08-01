#include "shell_erase.h"
#include "shell_util.h"
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

ShellErase::ShellErase(IProcessExecutor* executor) : executor_(executor) {}

bool ShellErase::Run(const std::string& input) {
	std::vector<std::string> commandVector = Util::splitBySpace(input);

	if (checkParameterValid(commandVector) == false) {
		Util::printInvalidParameter();
		return false;
	}

	int startLba = std::stoi(commandVector.at(START_LBA_INDEX));
	int curSize = std::stoi(commandVector.at(ERASE_SIZE_INDEX));

	calculateRangeAndPerformSSD(startLba, curSize);
	return true;
}

bool ShellErase::checkParameterValid(std::vector<std::string> commandVec) {
	// commandVec : 'earse', 'LBA', 'SIZE'
	if (commandVec.size() != VALID_ERASE_COMMAND_SIZE) {
		return false;
	}
	if (isDecimalString(commandVec.at(START_LBA_INDEX)) == false ||
		isDecimalString(commandVec.at(ERASE_SIZE_INDEX)) == false) {
		return false;
	}
	int inputLba;
	int inputSize;
	try {
		inputLba = std::stoi(commandVec.at(START_LBA_INDEX));
		inputSize = std::stoi(commandVec.at(ERASE_SIZE_INDEX));
	}
	catch (const std::invalid_argument&) {
		return false; // 숫자가 아님
	}
	catch (const std::out_of_range&) {
		return false; // 범위를 벗어남
	}
	if (inputLba < MIN_INDEX || inputLba >= MAX_INDEX) {
		return false;
	}
	return true;
}

void Erase::calculateRangeAndPerformSSD(int start, int size) {
	int cur = start;
	int left = std::abs(size);

	if (size == 0) return;

	if (size > 0) {
		// 양수: 위로(증가 방향)
		while (left > MIN_INDEX && cur < MAX_INDEX) {
			int chunk = std::min(MAX_SSD_ERASE_SIZE, left);
			if (cur + chunk > MAX_INDEX) {
				chunk = MAX_INDEX - cur;
			}
			if (chunk <= 0) {
				break;
			}
			performEraseToSSD(cur, chunk);

			cur += chunk;
			left -= chunk;
		}
	}
	else {
		while (left > MIN_INDEX && cur >= MIN_INDEX) {
			int chunk = std::min(MAX_SSD_ERASE_SIZE, left);
			if (cur - chunk + 1 < MIN_INDEX)
				chunk = cur + 1;
			if (chunk <= MIN_INDEX) break;

			performEraseToSSD(cur - chunk + 1, chunk);

			cur -= chunk;
			left -= chunk;
		}
	}
}

void ShellErase::performEraseToSSD(int index, int size) {
	std::string cmdLine;
	cmdLine = "SSDDriver.exe E " + std::to_string(index) + " " + std::to_string(size);
	executor_->Process(cmdLine);

#if(CONSOLE_TEST)
	std::cout << "(" << index << ", " << size << ")\n";
#endif
}

bool Erase::isDecimalString(const std::string& str) {
	if (str.empty()) return false;
	size_t i = 0;
	if (str[0] == '-' || str[0] == '+') i = 1;
	if (i == str.size()) return false; // 부호만 있는 경우
	for (; i < str.size(); ++i) {
		if (!std::isdigit(str[i])) return false;
	}
	return true;
}