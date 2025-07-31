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
	
	if (checkParameterValid(input) == false) {
		printInvalidCommand();
		return false;
	}
	
	std::stringstream ret;
	for (int i = MIN_INDEX; i < MAX_INDEX; i++) {
		std::string cmdLine = "SSDDriver.exe R " + std::to_string(i);
		executor_->Process(cmdLine);

		std::string SsdData = getSsdOutputData();

		ret << overWriteRead(i, SsdData);
	}

	std::cout << ret.str() << "\n";
	return true;
}

bool ShellFullRead::checkParameterValid(const std::string& input) {
	std::tuple<std::string, std::string> cmdTuple = parse_command(input);
	if (!std::get<1>(cmdTuple).empty()) {
		return false;
	}
	return true;
}

std::string ShellFullRead::overWriteRead(int index, const std::string& Value) {
	std::stringstream tempString;
	tempString << std::setw(2) << std::setfill('0') << index << " " << Value << "\n";
	return tempString.str();
}