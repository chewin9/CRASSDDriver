#include "flushcommand.h"

#include "command_factory.h"

void FlushCommand::Execute(const ParsedCommand&) {
  opHandler.Flush();
};

std::unique_ptr<ICommand> CreateFlushCommand(SsdOperationHandler& h) {
	return std::make_unique<FlushCommand>(h);
}

