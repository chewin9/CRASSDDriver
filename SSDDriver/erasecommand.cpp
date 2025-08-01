#include "erasecommand.h"

#include "command_factory.h"

bool EraseCommand::Execute(const ParsedCommand& cmdInfo) {
  bool result = opHandler.Erase(cmdInfo);
  return result;
}

ICommand* CreateEraseCommand(SsdOperationHandler& h) {
  return new EraseCommand(h);
}

namespace {
bool dummy = [] {
  CommandFactory::registerCommand("E", CreateEraseCommand);
  return true;
}();
}  // namespace