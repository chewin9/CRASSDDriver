#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "ishell_command.h"
#include "iprocess_executor.h"

class CommandInvoker {
public:
    void registerCommand(const std::string& name, std::shared_ptr<IShellCommand> command);
    bool executeCommand(const std::string& name, const std::string& input);
    void RegisterAllCommand(IProcessExecutor* executor);
private:
    std::unordered_map<std::string, std::shared_ptr<IShellCommand>> commands;
};