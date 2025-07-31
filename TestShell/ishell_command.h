#pragma once
#include <string>
#define Interface struct 

Interface IShellCommand {
	virtual bool Run(const std::string & cmd) = 0;
};