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
        shell_read = new ShellRead(&executor);
        shell_write = new ShellWrite(&executor);
        shell_fullwrite = new ShellFullWrite(&executor);
        script_runner = new TestScriptRunner(&executor);
        shell_help = new ShellHelp(&executor);
    };

    Shell(IProcessExecutor * exe) {
        shell_read = new ShellRead(exe);
        shell_write = new ShellWrite(exe);
        shell_fullwrite = new ShellFullWrite(exe);
        script_runner = new TestScriptRunner(exe);
    }

    int Run(void);
    virtual string get_command(string& input);

private:
    ProcessExecutor executor;
    ShellWrite* shell_write;
    ShellFullWrite* shell_fullwrite;
    ShellRead* shell_read;
    ShellHelp* shell_help;

    TestScriptRunner* script_runner;

    std::tuple<std::string, std::string> parse_command(const std::string& input);
};