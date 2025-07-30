#pragma once
#include "iprocess_executor.h"

class ProcessExecutor : public IProcessExecutor {
public:
    int Process(const std::string& commandLine) override;
};