#pragma once
#include "iprocess_executor.h"

class ProcessExecutor : public IProcessExecutor {
public:
    void readExecutor(const std::string& command) override;
};