#include <string>
#pragma once
#include "command_parser.h"

class ICommand {

public:
	virtual ~ICommand() = default;
	virtual bool Execute() = 0;
};
