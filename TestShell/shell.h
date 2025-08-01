#pragma once

#include "testscript.h"
#include "process_executor.h"
#include "File.h"

class Shell {
public:
    Shell() {
        executor = new ProcessExecutor();
        script_runner = new TestScriptRunner(executor, &testscriptfile);
    };

    Shell(IProcessExecutor * exe) {
        executor = exe;
        script_runner = new TestScriptRunner(exe, nullptr);
    }

    int Run(std::string arg = "");
    virtual std::string get_command(std::string& input);
    IProcessExecutor* Get_Executor() { return executor; }

private:
    IProcessExecutor* executor;
    File testscriptfile;
    TestScriptRunner* script_runner;

    std::tuple<std::string, std::string> parse_command(const std::string& input);
};