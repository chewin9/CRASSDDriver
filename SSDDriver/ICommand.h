#pragma once

#include <string>

class ICommand {

public:
	virtual ~ICommand() = default;
	virtual bool Execute(int lba, std::string value) = 0;
};
