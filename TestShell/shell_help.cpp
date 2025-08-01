#include "shell_help.h"
#include "shell_util.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

ShellHelp::ShellHelp(IProcessExecutor* executor) : executor_(executor) {}

bool ShellHelp::Run(const std::string& input) {
	std::vector<std::string> commandVector = Util::splitBySpace(input);

	if (commandVector.size() == INTRODUCE_COMMAND_SIZE) {
		crewIntroduce();
		helpCommand();
	}
	else if (commandVector.size() == VALID_HELP_COMMAND_SIZE) {
		if (commandVector.at(HELP_COMMAND_IDEX) == "write") {
			writeCommand();
		}
		else if (commandVector.at(HELP_COMMAND_IDEX) == "read") {
			readCommand();
		}
		else if (commandVector.at(HELP_COMMAND_IDEX) == "exit") {
			exitCommand();
		}
		else if (commandVector.at(HELP_COMMAND_IDEX) == "fullwrite") {
			fullwriteCommand();
		}
		else if (commandVector.at(HELP_COMMAND_IDEX) == "fullread") {
			fullreadCommand();
		}
		else if (commandVector.at(HELP_COMMAND_IDEX) == "erase") {
			eraseCommand();
		}
		else if (commandVector.at(HELP_COMMAND_IDEX) == "erase_range") {
			eraserangeCommand();
		}
		else if (commandVector.at(HELP_COMMAND_IDEX) == "flush") {
			flushCommand();
		}
		else {
			helpCommand();
		}
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
	std::cout << "  -help erase\n";
	std::cout << "  -help erase_range\n";
	std::cout << "  -help flush\n";

}
void ShellHelp::writeCommand(void) {
	std::cout << "write command: write [LBA] [Value]\n";
	std::cout << "  - \"write\" must be in lowercase.\n";
	std::cout << "  - [LBA]: 0 ~ 99\n";
	std::cout << "  - [Value]:It must always start with 0x and be written as a 10-character string.\n";
	std::cout << "  - ex) write 0 0x12345678\n";
}
void ShellHelp::readCommand(void) {
	std::cout << "read command: read [LBA]\n";
	std::cout << "  - \"read\" must be in lowercase.\n";
	std::cout << "  - [LBA]: 0 ~ 99\n";
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
void ShellHelp::eraseCommand(void) {
	std::cout << "erase command: erase [LBA] [SIZE]\n";
	std::cout << "  - \"erase\" must be in lowercase.\n";
	std::cout << "  - Delete content from a specific LBA to a specific SIZE.\n";
	std::cout << "  - [LBA]: 0 ~ 99\n";
	std::cout << "  - [SIZE]: SIZE can be negative, positive input\n";
	std::cout << "            Delete down from LBA if negative\n";
	std::cout << "            Delete up from LBA if positive\n";
	std::cout << "  - ex) erase 10 30, erase 30 -10\n";
}

void ShellHelp::eraserangeCommand(void) {
	std::cout << "erase_range command: erase [Start LBA] [End LBA]\n";
	std::cout << "  - \"erase_range\" must be in lowercase.\n";
	std::cout << "  - Delete content from a Start LBA to End LBA.\n";
	std::cout << "  - Start LBA and end LBA are not in order.\n";
	std::cout << "  - [Start LBA]: 0 ~ 99\n";
	std::cout << "  - [End LBA]: 0 ~ 99\n";
	std::cout << "  - ex) erase_range 10 20, erase_range 30 20\n";
}
void ShellHelp::flushCommand(void) {
	std::cout << "flush command: flush\n";
	std::cout << "  - \"flush\" must be in lowercase.\n";
	std::cout << "  - Run all commands in the command buffer to empty the buffer.\n";
	std::cout << "  - ex) flush\n";
}