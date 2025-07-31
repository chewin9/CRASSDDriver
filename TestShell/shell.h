#pragma once

#include "shell_read.h"
#include "shell_write.h"
#include "shell_full_write.h"
#include "testscript.h"
#include "shell_help.h"
#include "process_executor.h"

class Shell {
public:
    Shell() {
        executor = new ProcessExecutor();
        shell_read = new ShellRead(executor);
        script_runner = new TestScriptRunner(executor);
        shell_help = new ShellHelp(executor);
    };

    Shell(IProcessExecutor * exe) {
        executor = exe;
        shell_read = new ShellRead(exe);
        script_runner = new TestScriptRunner(exe);
    }

    int Run(void);
    virtual string get_command(string& input);

private:
    IProcessExecutor* executor;
    ShellRead* shell_read;
    ShellHelp* shell_help;

    TestScriptRunner* script_runner;

    std::tuple<std::string, std::string> parse_command(const std::string& input);
};