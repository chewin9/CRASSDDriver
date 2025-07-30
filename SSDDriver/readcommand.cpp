#include "readcommand.h"
#include "command_parser.h"

bool ReadCommand::Execute() {
  bool result = fileio.ReadNandFile(cmdInfo);
  return result;
}