#include "flushcommand.h"

bool FlushCommand::Execute(const ParsedCommand &) {
	opHandler.Flush();
	return true;
};