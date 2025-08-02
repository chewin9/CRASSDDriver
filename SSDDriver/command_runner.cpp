#include "command_runner.h"
#include "command_factory.h"
#include <memory>

CommandRunner::CommandRunner(CommandParser& p, FileIO& f, CommandBuffer& c, SsdOperationHandler& o)
    : parser{ p }, fileio{ f }, cmdBuffer{ c }, opHandler{ o } {
}

void CommandRunner::Run(int argc, char* argv[]) {
    ParsedCommand cmdInfo = parser.ParseCommand(argc, argv);

    if (cmdInfo.errorFlag) {
        fileio.WriteValueToOutputFile("ERROR");
        return;
    }

    std::unique_ptr<ICommand> command(CommandFactory::create(cmdInfo.opCode, opHandler));

    if (!command) {
        fileio.WriteValueToOutputFile("ERROR");
        return;
    }

    command->Execute(cmdInfo);
}