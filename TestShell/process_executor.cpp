#include "process_executor.h"
#include <cstdlib>

void ProcessExecutor::Process(const std::string& commandLine) {
    system(commandLine.c_str());
}