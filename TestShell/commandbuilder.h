#pragma once
#include <string>

const std::string SSD_NAME = "SSDDriver.exe";

class CommandBuilder {
public:
	std::string makeWriteCommand(unsigned int addr, unsigned int value);
	std::string makeReadCommand(unsigned int addr);
	std::string makeEraseCommand(unsigned int addr, unsigned int size);
private:
	std::string GetSSDName() { return SSD_NAME; }
};
