#pragma once
#include "iprocess_executor.h"
#include <memory>

class ShellWrite {
public:
    ShellWrite(IProcessExecutor* executor) : executor_(executor) {}

    void IssueWrite(const std::string& input) {
        // parse input (ex: write 3 0xAAAABBBB)
        if (input.find("write ") == 0) {
            std::string cmdLine = "ssd.exe W " + input.substr(6);
            executor_->Process(cmdLine);
        }
    }

private:
    IProcessExecutor* executor_;
};