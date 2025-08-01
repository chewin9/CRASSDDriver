#pragma once
#include "icommand.h"
#include "ssd_operation_handler.h"

ICommand* CreateReadCommand(SsdOperationHandler& handler);
ICommand* CreateWriteCommand(SsdOperationHandler& handler);
ICommand* CreateEraseCommand(SsdOperationHandler& handler);
ICommand* CreateFlushCommand(SsdOperationHandler& handler);