#include "process_executor.h"
#include <cstdlib>

void ProcessExecutor::readExecutor(const std::string& command) {
    system(command.c_str());
}