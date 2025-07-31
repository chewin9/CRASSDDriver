#include "readcommand.h"
#include "command_parser.h"

bool ReadCommand::Execute(const ParsedCommand& cmdInfo) {
  bool result = opHandler.Read(cmdInfo);
  return result;
}