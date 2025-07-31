#include "writecommand.h"

bool WriteCommand::Execute(const ParsedCommand& cmdInfo) {
  opHandler.Write(cmdInfo);
	return true;
};