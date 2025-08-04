#include "erasecommand.h"

#include "command_factory.h"

void EraseCommand::Execute(const ParsedCommand& cmdInfo) {
	opHandler.WriteAndErase(cmdInfo);
}

std::unique_ptr<ICommand> CreateEraseCommand(SsdOperationHandler& h) {
	return std::make_unique<EraseCommand>(h);
}
