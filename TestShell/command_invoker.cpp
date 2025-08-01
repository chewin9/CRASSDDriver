#include <unordered_map>
#include <memory>
#include "shell_read.h"
#include "shell_write.h"
#include "shell_full_write.h"
#include "shell_full_read.h"
#include "shell_flush.h"
#include "shell_erase.h"
#include "shell_erase_range.h"
#include "shell_help.h"
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

void CommandInvoker::RegisterAllCommand(IProcessExecutor* executor)
{
    this->registerCommand("write", std::make_shared<ShellWrite>(executor));
    this->registerCommand("fullwrite", std::make_shared<ShellFullWrite>(executor));
    this->registerCommand("read", std::make_shared<ShellRead>(executor));
    this->registerCommand("fullread", std::make_shared<ShellFullRead>(executor));
    this->registerCommand("flush", std::make_shared<ShellFlush>(executor));
    this->registerCommand("help", std::make_shared<ShellHelp>(executor));
    this->registerCommand("erase", std::make_shared<ShellErase>(executor));
    this->registerCommand("erase_range", std::make_shared<ShellEraseRange>(executor));
}