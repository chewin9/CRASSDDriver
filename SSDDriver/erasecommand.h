#pragma once

#include "ICommand.h"
#include "ssd_operation_handler.h"

class EraseCommand : public ICommand {
    SsdOperationHandler& opHandler;

public:
    EraseCommand(SsdOperationHandler& opHandler)
        : opHandler{ opHandler } {
    }
    bool Execute(const ParsedCommand& cmdInfo) override;
};