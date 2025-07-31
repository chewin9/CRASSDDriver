#pragma once

#include "shell_read.h"
#include "shell_write.h"
#include "shell_full_write.h"
#include "shell_full_read.h"
#include "testscript.h"
#include "shell_help.h"
#include "process_executor.h"

class Shell {
public:
    Shell() {
        executor = new ProcessExecutor();
        script_runner = new TestScriptRunner(executor);
    };

    Shell(IProcessExecutor * exe) {
        executor = exe;
        script_runner = new TestScriptRunner(exe);
    }

    int Run(std::string arg = "");
    virtual string get_command(string& input);

private:
    IProcessExecutor* executor;

    TestScriptRunner* script_runner;

    std::tuple<std::string, std::string> parse_command(const std::string& input);
};