#include "command_runner.h"
#include "command_factory.h"
#include <memory>

CommandRunner::CommandRunner(CommandParser& p, FileIO& f)
    : parser{ p }, fileio{ f } {
}

void CommandRunner::Run(int argc, char* argv[]) {
    ParsedCommand cmdInfo = parser.ParseCommand(argc, argv);
    std::unique_ptr<ICommandOptimizer> opt;
    if (cmdInfo.opCode == "W") {
        opt = std::make_unique<WriteCommandOptimizer>();
    }
    else if (cmdInfo.opCode == "E") {
        opt = std::make_unique<EraseCommandOptimizer>();
    }

    CommandBuffer buffer(std::move(opt));
    if (cmdInfo.errorFlag) {
        fileio.WriteValueToOutputFile("ERROR");
        return;
    }
    SsdOperationHandler opHandler{ fileio, buffer };
    std::unique_ptr<ICommand> command(CommandFactory::create(cmdInfo.opCode, opHandler));

    command->Execute(cmdInfo);
}