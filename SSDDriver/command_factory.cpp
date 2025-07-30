#include "command_factory.h"
#include "readcommand.h"
#include "writecommand.h"

ICommand* CommandFactory::create(ParsedCommand& cmd) {
  if (cmd.opCode == "W") {
    return new WriteCommand(cmd);
  } else if (cmd.opCode == "R") {
    return new ReadCommand(cmd);
  }
  return nullptr;
}