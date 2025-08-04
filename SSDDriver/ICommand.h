#pragma once
#include "command_parser.h"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void Execute(const ParsedCommand& cmdInfo) = 0;
};