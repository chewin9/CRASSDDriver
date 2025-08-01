#include "flushcommand.h"

#include "command_factory.h"

bool FlushCommand::Execute(const ParsedCommand&) {
  opHandler.Flush();
  return true;
};

ICommand* CreateFlushCommand(SsdOperationHandler& h) {
  return new FlushCommand(h);
}

namespace {
bool dummy = [] {
  CommandFactory::registerCommand("F", CreateFlushCommand);
  return true;
}();
}  // namespace