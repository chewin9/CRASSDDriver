#include "writecommand.h"

#include "command_factory.h"

void WriteCommand::Execute(const ParsedCommand& cmdInfo) {
  opHandler.WriteAndErase(cmdInfo);
};

std::unique_ptr<ICommand> CreateWriteCommand(SsdOperationHandler& h) {
	return std::make_unique<WriteCommand>(h);
}


