#include "command_factory.h"
#include "readcommand.h"
#include "writecommand.h"
#include "erasecommand.h"
#include "ssd_operation_handler.h"

ICommand* CommandFactory::create(const ParsedCommand& cmd, SsdOperationHandler& opHandler) {
  if (cmd.opCode == "W") {
    return new WriteCommand(opHandler);
  }
  else if (cmd.opCode == "R") {
    return new ReadCommand(opHandler);
  }
  else if (cmd.opCode == "E") {
      return new EraseCommand(opHandler);
  }
  return nullptr;
}