#include "shell_read.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

ShellRead::ShellRead(IProcessExecutor* executor) : executor_(executor) {}

bool ShellRead::Run(const std::string& input) {
	if (checkParameterValid(input) == false) {
		printInvalidCommand();
		return false;
	}

	// Parse 하는 부분
	std::tuple<std::string, std::string> parseCommand = parse_command(input);
	
	//SSD 수행
	std::string commandLBA = std::get<1>(parseCommand);
	std::string cmdLine = "SSDDriver.exe R " + commandLBA;
	executor_->Process(cmdLine);
	
	//결과 값 읽기
	std::string SsdData = getSsdOutputData();
	int index = std::stoi(commandLBA);
	printResult(index, SsdData);

	return true;
}

std::string Read::getSsdOutputData(void) {

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

void Read::printInvalidCommand() {
	std::cout << INVALID_COMMAND << "\n";
}

void ShellRead::printResult(int index, std::string value) {
	if (value == ERROR_RETURN) {
		std::cout << ERROR_RETURN << "\n";
	}
	else {
		std::cout << "[Read] LBA ";
		std::cout << std::setw(2) << std::setfill('0') << index << " ";
		std::cout << value << "\n";
	}
}

bool ShellRead::checkParameterValid(const std::string& cmd) {
	std::tuple<std::string, std::string> cmdTuple = parse_command(cmd);
	if (std::get<1>(cmdTuple).empty() == true) {
		return false;
	}
	return true;
}

std::tuple<std::string, std::string> Read::parse_command(const std::string& input) {
	std::istringstream iss(input);
	std::string cmd;
	std::string num_str;
	iss >> cmd >> num_str;
	return { cmd, num_str };
}