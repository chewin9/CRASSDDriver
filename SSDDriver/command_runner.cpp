#include "command_runner.h"

CommandRunner::CommandRunner() {
  opHandler = new SsdOperationHandler(fileio, cmdBuffer);
}

CommandRunner::~CommandRunner() { delete opHandler; }

void CommandRunner::Run(int argc, char* argv[]) {
  ParsedCommand cmdInfo = parser.ParseCommand(argc, argv);

  ICommand* command = CommandFactory::create(cmdInfo.opCode, *opHandler);
  if (!command) {
    fileio.WriteValueToOutputFile("ERROR");  // Optional error handling
    return;
  }

  command->Execute(cmdInfo);
  delete command;
}
