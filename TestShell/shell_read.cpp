#include "shell_read.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

ShellRead::ShellRead(IProcessExecutor* executor) : executor_(executor) {}

void ShellRead::fullRead(const std::string input) {

	std::tuple<std::string, std::string> parseCommand = parse_command(input);
	if (!std::get<1>(parseCommand).empty()) {
		printInvalidCommand();
		return;
	}

	std::string ret;
	ret = appendSSDData();

	std::cout << ret << "\n";
}

void ShellRead::read(const std::string input) {
	// Parse 하는 부분
	std::tuple<std::string, std::string> parseCommand = parse_command(input);

	if (std::get<1>(parseCommand).empty()) {
		printInvalidCommand();
		return;
	}
	
	//SSD 수행
	std::string commandLine = std::get<1>(parseCommand);
	runSSDDriver(commandLine);
	
	//결과 값 읽기
	std::string SsdData = getSsdOutputData();


	int index = std::stoi(commandLine);
	printResult(index, SsdData);
}

std::tuple<std::string, std::string> ShellRead::parse_command(const std::string& input) {
	std::istringstream iss(input);
	std::string cmd;
	std::string num_str;
	iss >> cmd >> num_str;
	return { cmd, num_str };
}

std::string ShellRead::getSsdOutputData(void) {

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

bool ShellRead::isInvalidIndex(int index) {
	if (index < MIN_INDEX || index >= MAX_INDEX) {
		return true;
	}
	return false;
}

std::string ShellRead::appendSSDData(void) {
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

void ShellRead::printInvalidCommand() {
	std::cout << INVALID_COMMAND << "\n";
}

void ShellRead::runSSDDriver(std::string index) {
	std::string cmdLine = "SSDDriver.exe R " + index;
	executor_->Process(cmdLine);
}

void ShellRead::printError() {
	std::cout << ERROR_RETURN << "\n";
}

void ShellRead::printResult(int index, std::string value) {
	if (value == ERROR_RETURN) {
		printError();
	}
	else {
		std::cout << "[Read] LBA ";
		std::cout << std::setw(2) << std::setfill('0') << index << " ";
		std::cout << value << "\n";
	}
}
