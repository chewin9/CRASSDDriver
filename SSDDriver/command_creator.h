#pragma once
#include "icommand.h"
#include "ssd_operation_handler.h"

std::unique_ptr<ICommand> CreateReadCommand(SsdOperationHandler& h);
std::unique_ptr<ICommand> CreateWriteCommand(SsdOperationHandler& h);
std::unique_ptr<ICommand> CreateEraseCommand(SsdOperationHandler& h);
std::unique_ptr<ICommand> CreateFlushCommand(SsdOperationHandler& h);