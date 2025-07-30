#pragma once

#include "shell_write.h"
#include "shell_full_write.h"
#include "process_executor.h"

class Shell {
public:
    Shell() {
        shell_write = new ShellWrite(&executor);
        shell_fullwrite = new ShellFullWrite(&executor);
    };
    int Run(void);

private:
    ProcessExecutor executor;
    ShellWrite* shell_write;
    ShellFullWrite* shell_fullwrite;
};