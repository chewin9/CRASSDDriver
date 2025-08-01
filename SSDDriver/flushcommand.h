#include <string>
#pragma once
#include "ICommand.h"
#include "ssd_operation_handler.h"

class FlushCommand : public ICommand {
    SsdOperationHandler& opHandler;

public:
    FlushCommand(SsdOperationHandler& opHandler)
        : opHandler{ opHandler } {
    }
    bool Execute(const ParsedCommand& cmdInfo) override;
};
