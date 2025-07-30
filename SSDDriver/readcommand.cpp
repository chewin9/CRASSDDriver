#include "readcommand.h"
#include "command_parser.h"

bool ReadCommand::Execute() {
  fileio.ReadNandFile(cmdInfo);
  return true;
}