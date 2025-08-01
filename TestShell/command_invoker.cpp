#include "command_invoker.h"

void CommandInvoker::registerCommand(const std::string& name, std::shared_ptr<IShellCommand> command) {
    commands[name] = command;
}

bool CommandInvoker::executeCommand(const std::string& name, const std::string& input) {
    auto it = commands.find(name);
    if (it != commands.end()) {
        it->second->Run(input);
        return true;
    }
    return false;
}