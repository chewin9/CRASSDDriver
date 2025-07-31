#include "shell_help.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

ShellHelp::ShellHelp(IProcessExecutor* executor) : executor_(executor) {}

bool ShellHelp::Run(const std::string &input) {
	std::tuple<std::string, std::string> parseCommand = parse_command(input);

	if (std::get<1>(parseCommand).empty()) {
		crewIntroduce();
		helpCommand();
	}else if(std::get<1>(parseCommand) == "write"){
		writeCommand();
	}
	else if (std::get<1>(parseCommand) == "read") {
		readCommand();
	}
	else if (std::get<1>(parseCommand) == "exit") {
		exitCommand();
	}
	else if (std::get<1>(parseCommand) == "fullwrite") {
		fullwriteCommand();
	}
	else if (std::get<1>(parseCommand) == "fullread") {
		fullreadCommand();
	}
	else {
		helpCommand();
	}
	return true;
}

void ShellHelp::crewIntroduce() {
	std::cout << "ÆÀ¸í: All - pass\n";
	std::cout << "ÆÀ¿ø : ÀÌ½ÂÃ¶(ÆÀÀå), Áø¿¬±â, ¹ÚÁ¤±Ù, ÀÌÁØ¼®, ±è¿¹¼Ö, °­ÀÎÇý\n";
}
void ShellHelp::helpCommand(void) {
	std::cout << "How to use help command\n";
	std::cout << "  -help write\n";
	std::cout << "  -help read\n";
	std::cout << "  -help exit\n";
	std::cout << "  -help fullwrite\n";
	std::cout << "  -help fullread\n";
}
void ShellHelp::writeCommand(void) {
	std::cout << "write command: write [LBA] [Value]\n";
	std::cout << "  - \"write\" must be in lowercase.\n";
	std::cout << "  - [LBA]: 0 ~ 100\n";
	std::cout << "  - [Value]:It must always start with 0x and be written as a 10-character string.\n";
	std::cout << "  - ex) write 0 0x12345678\n";
}
void ShellHelp::readCommand(void) {
	std::cout << "read command: read [LBA]\n";
	std::cout << "  - \"read\" must be in lowercase.\n";
	std::cout << "  - [LBA]: 0 ~ 100\n";
	std::cout << "  - ex) read 0 \n";
}
void ShellHelp::exitCommand(void) {
	std::cout << "exit command: exit\n";
	std::cout << "  - Test Shell is terminated.\n";
	std::cout << "  - ex) exit\n";
}
void ShellHelp::fullwriteCommand(void) {
	std::cout << "fullwrite command: fullwrite [Value]\n";
	std::cout << "  - A write operation is performed on all LBA areas.\n";
	std::cout << "  - ex) fullwrite 0x12345678\n";
}
void ShellHelp::fullreadCommand(void) {
	std::cout << "fullread command: fullread\n";
	std::cout << "  - A read operation is performed from LBA 0 to 99.\n";
	std::cout << "  - All values of the SSD are displayed on the screen.\n";
	std::cout << "  - ex) fullread\n";
}


std::tuple<std::string, std::string> ShellHelp::parse_command(const std::string& input) {
	std::istringstream iss(input);
	std::string cmd;
	std::string cmd2;
	iss >> cmd >> cmd2;
	return { cmd, cmd2 };
}