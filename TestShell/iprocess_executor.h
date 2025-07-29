#pragma once
#include <string>

class IProcessExecutor {
public:
    virtual ~IProcessExecutor() = default;
    virtual void Process(const std::string& command) = 0;
};