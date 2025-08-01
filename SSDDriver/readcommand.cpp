#include "readcommand.h"

#include "command_factory.h"

bool ReadCommand::Execute(const ParsedCommand& cmdInfo) {
  bool result = opHandler.Read(cmdInfo);
  return result;
}

ICommand* CreateReadCommand(SsdOperationHandler& h) {
  return new ReadCommand(h);
}
