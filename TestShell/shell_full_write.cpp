#pragma once
#include "iprocess_executor.h"
#include "shell_full_write.h"
#include "shell_util.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

bool ShellFullWrite::Run(const std::string& cmd) {

	if (false == checkParameterValid(cmd)) {
		Util::printInvalidParameter();
		return false;
	}

	std::vector<std::string> cmds = Util::splitBySpace(cmd);
	std::string value = cmds[1];

	for (int nLBA = START_LBA; nLBA < END_LBA; nLBA++) {
		std::string cmdLine = "SSDDriver.exe W " + std::to_string(nLBA) + " " + value;
		executor_->Process(cmdLine);
	}
	std::cout << "[WriteFull] Done" << std::endl;
	return true;
}

bool ShellFullWrite::checkParameterValid(const std::string& cmd)
{
	std::vector<std::string> cmds = Util::splitBySpace(cmd);
	if (cmds.size() != 2) {
		return false;
	}

	std::string value = cmds[1];
	if (value.size() != 10) {
		return false;
	}

	if (value.find("0x") != 0) {
		return false;
	}

	if (false == is_all_uppercase(value.substr(2, 8))){
		return false;
	}

	if (false == is_valid_unsigned(value)) {
		return false;
	}
	return true;
}
