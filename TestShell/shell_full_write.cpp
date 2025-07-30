#pragma once
#include "iprocess_executor.h"
#include "shell_full_write.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

const int START_LBA = 0;
const int END_LBA = 100;

void ShellFullWrite::IssueFullWrite(std::string cmd) 
{
	std::vector<std::string> cmds = splitBySpace(cmd);

	if (cmds.size() != 2) {
		std::cout << "INVALID_COMMAND" << std::endl;
		return;
	}

	std::string value = cmds[1];
	if (value.size() != 10) {
		std::cout << "INVALID_COMMAND" << std::endl;
		return;
	}

	if (value.find("0x") != 0) {
		std::cout << "INVALID_COMMAND" << std::endl;
		return;
	}

	if (false == is_valid_unsigned(value)){
		std::cout << "INVALID_COMMAND" << std::endl;
		return;
	}

	for (int nLBA = START_LBA; nLBA < END_LBA; nLBA++) {
		std::string cmdLine = "ssd.exe W " + std::to_string(nLBA) + " " + value;
		executor_->Process(cmdLine);
	}
	std::cout << "[WriteFull] Done" << std::endl;
}

std::vector<std::string> ShellFullWrite::splitBySpace(const std::string& str) {
	std::istringstream iss(str);
	std::vector<std::string> tokens;
	std::string word;
	while (iss >> word) {
		tokens.push_back(word);
	}
	return tokens;
}


bool ShellFullWrite::is_valid_unsigned(const std::string& str) {
	size_t idx = 0;
	try {
		std::stoul(str, &idx, 0); // base 0: auto-detect base (hex, dec, oct)
		return idx == str.size(); // true if entire string was used
	}
	catch (const std::invalid_argument&) {
		return false; // Not a number at all
	}
	catch (const std::out_of_range&) {
		return false; // Number outside unsigned long range
	}
}
