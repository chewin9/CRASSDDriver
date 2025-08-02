#include "readcommand.h"

#include "command_factory.h"

void ReadCommand::Execute(const ParsedCommand& cmdInfo) {
	opHandler.Read(cmdInfo);
}

std::unique_ptr<ICommand> CreateReadCommand(SsdOperationHandler& h) {
	return std::make_unique<ReadCommand>(h);
}

