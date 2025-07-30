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
    int Run(void);

private:
    ProcessExecutor executor;
    ShellWrite* shell_write;
    ShellFullWrite* shell_fullwrite;
    ShellRead* shell_read;
    ShellHelp* shell_help;

    TestScriptRunner* script_runner;
};