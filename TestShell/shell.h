#pragma once

#include <unordered_map>
#include <memory>
#include "shell_read.h"
#include "shell_write.h"
#include "shell_full_write.h"
#include "shell_full_read.h"
#include "shell_flush.h"
#include "shell_erase.h"
#include "shell_erase_range.h"
#include "testscript.h"
#include "shell_help.h"
#include "process_executor.h"
#include "File.h"

class CommandInvoker {
public:
    void registerCommand(const std::string& name, std::shared_ptr<IShellCommand> command) {
        commands[name] = command;
    }

    bool executeCommand(const std::string& name, const std::string& input) {
        auto it = commands.find(name);
        if (it != commands.end()) {
            it->second->Run(input);
            return true;
        }
        return false;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<IShellCommand>> commands;
};

class Shell {
public:
    Shell() {
        executor = new ProcessExecutor();
        script_runner = new TestScriptRunner(executor, &testscriptfile);
        initializeCommands();
    };

    Shell(IProcessExecutor * exe) {
        executor = exe;
        script_runner = new TestScriptRunner(exe, nullptr);
        initializeCommands();
    }

    int Run(std::string arg = "");
    virtual string get_command(string& input);

private:
    IProcessExecutor* executor;
    File testscriptfile;
    TestScriptRunner* script_runner;
    CommandInvoker invoker;

    void initializeCommands() {
        invoker.registerCommand("write", std::make_shared<ShellWrite>(executor));
        invoker.registerCommand("fullwrite", std::make_shared<ShellFullWrite>(executor));
        invoker.registerCommand("read", std::make_shared<ShellRead>(executor));
        invoker.registerCommand("fullread", std::make_shared<ShellFullRead>(executor));
        invoker.registerCommand("flush", std::make_shared<ShellFlush>(executor));
        invoker.registerCommand("help", std::make_shared<ShellHelp>(executor));
        invoker.registerCommand("erase", std::make_shared<ShellErase>(executor));
        invoker.registerCommand("erase_range", std::make_shared<ShellEraseRange>(executor));
    }

    std::tuple<std::string, std::string> parse_command(const std::string& input);
};