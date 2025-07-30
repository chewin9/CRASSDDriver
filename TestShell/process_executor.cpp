#include "process_executor.h"
#include <cstdlib>

int ProcessExecutor::Process(const std::string& commandLine) {
    return system(commandLine.c_str());
}