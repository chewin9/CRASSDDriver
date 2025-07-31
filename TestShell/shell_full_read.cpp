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

void ShellFullRead::fullread(const std::string input) {
	std::tuple<std::string, std::string> parseCommand = parse_command(input);
	if (!std::get<1>(parseCommand).empty()) {
		printInvalidCommand();
		return;
	}
	std::string ret;
	ret = appendSSDData();

	std::cout << ret << "\n";
}

std::tuple<std::string, std::string> ShellFullRead::parse_command(const std::string& input) {
	std::istringstream iss(input);
	std::string cmd;
	std::string num_str;
	iss >> cmd >> num_str;
	return { cmd, num_str };
}

void ShellFullRead::printInvalidCommand() {
	std::cout << INVALID_COMMAND << "\n";
}

std::string ShellFullRead::appendSSDData(void) {
	std::stringstream ret;
	for (int i = MIN_INDEX; i < MAX_INDEX; i++) {
		runSSDDriver(std::to_string(i));

		std::string readSSDData = getSsdOutputData();
		ret << std::setw(2) << std::setfill('0') << i << " ";
		ret << readSSDData;
		ret << "\n";
	}

	return ret.str();
}

void ShellFullRead::runSSDDriver(std::string index) {
	std::string cmdLine = "SSDDriver.exe R " + index;
	executor_->Process(cmdLine);
}

std::string ShellFullRead::getSsdOutputData(void) {

	std::ifstream file("ssd_output.txt");
	if (!file.is_open()) {
		std::cout << "open fail\n";
		return "";
	}
	std::string ret;
	std::getline(file, ret);
	file.close();

	return ret;
}