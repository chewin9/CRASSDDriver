#include "command_factory.h"
#include "readcommand.h"
#include "writecommand.h"

ICommand* CommandFactory::create(ParsedCommand& cmd, FileIO& fileio) {
  if (cmd.opCode == "W") {
    return new WriteCommand(cmd, fileio);
  } else if (cmd.opCode == "R") {
    return new ReadCommand(cmd, fileio);
  }
  return nullptr;
}