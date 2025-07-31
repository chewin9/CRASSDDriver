#include "shell_erase.h"
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

ShellErase::ShellErase(IProcessExecutor* executor) : executor_(executor) {}

bool ShellErase::Run(const std::string& input) {
	std::vector<std::string> commandVector = splitBySpace(input);

	if (checkParameterValid(commandVector) == false) {
		printInvalidCommand();
		return false;
	}

	int startLba = std::stoi(commandVector.at(1));
	int curSize = std::stoi(commandVector.at(2));

	calculateRangeAndPerformSSD(startLba, curSize);
	return true;
}

std::vector<std::string> Erase::splitBySpace(const std::string& cmd) {
	std::istringstream iss(cmd);
	std::vector<std::string> tokens;
	std::string word;
	while (iss >> word) {
		tokens.push_back(word);
	}
	return tokens;
}

void Erase::printInvalidCommand() {
	std::cout << INVALID_COMMAND << "\n";
}

bool ShellErase::checkParameterValid(std::vector<std::string> commandVec) {
	// commandVec : 'earse', 'LBA', 'SIZE'
	if (commandVec.size() != 3) {
		return false;
	}
	int inputLba = std::stoi(commandVec.at(1));
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
		while (left > 0 && cur < 100) {
			int chunk = std::min(10, left);
			if (cur + chunk > 100) {
				chunk = 100 - cur;
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
		while (left > 0 && cur >= 0) {
			int chunk = std::min(10, left);
			if (cur - chunk + 1 < 0)
				chunk = cur + 1;
			if (chunk <= 0) break;

			performEraseToSSD(cur - chunk + 1, chunk);

			cur -= chunk;
			left -= chunk;
		}
	}
}

void ShellErase::performEraseToSSD(int index, int size) {
	std::string cmdLine;
	cmdLine = "SSDDriver.exe R " + std::to_string(index) + " " + std::to_string(size);
	executor_->Process(cmdLine);

#if(CONSOLE_TEST)
	std::cout << "(" << index << ", " << size << ")\n";
#endif
}