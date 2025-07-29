#pragma once
#include "iprocess_executor.h"

class ProcessExecutor : public IProcessExecutor {
public:
    void Process(const std::string& commandLine) override;
};