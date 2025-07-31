#include "erasecommand.h"

bool EraseCommand::Execute(const ParsedCommand& cmdInfo) {

	bool result = opHandler.Erase(cmdInfo);
	return result;
}
