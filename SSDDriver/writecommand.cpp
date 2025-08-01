#include "writecommand.h"

#include "command_factory.h"

bool WriteCommand::Execute(const ParsedCommand& cmdInfo) {
  opHandler.Write(cmdInfo);
  return true;
};

ICommand* CreateWriteCommand(SsdOperationHandler& h) {
  return new WriteCommand(h);
}

