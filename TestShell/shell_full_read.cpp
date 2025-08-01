#pragma once
#include "iprocess_executor.h"
#include "shell_full_read.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>

ShellFullRead::ShellFullRead(IProcessExecutor* executor) : executor_(executor) {}

bool ShellFullRead::Run(const std::string& input) {
	std::vector<std::string> commandVector = splitBySpace(input);

	if (checkParameterValid(commandVector) == false) {
		printInvalidParameter();
		return false;
	}

	std::stringstream ret;
	for (int i = MIN_INDEX; i < MAX_INDEX; i++) {
		performReadToSSD(std::to_string(i));

		std::string SsdData = getSsdOutputData();
		ret << appendSsdData(i, SsdData);
	}

	std::cout << ret.str() << "\n";
	return true;
}

bool ShellFullRead::checkParameterValid(std::vector<std::string> commandVec) {
	if (commandVec.size() != VALID_FULLREAD_COMMAND_SIZE) {
		return false;
	}
	return true;
}

std::string ShellFullRead::appendSsdData(int index, const std::string& Value) {
	std::stringstream tempString;
	tempString << std::setw(2) << std::setfill('0') << index << " " << Value << "\n";
	return tempString.str();
}

void ShellFullRead::performReadToSSD(std::string index) {
	std::string cmdLine = "SSDDriver.exe R " + index;
	executor_->Process(cmdLine);
}