#pragma once
#include <string>

class IProcessExecutor {
public:
    virtual ~IProcessExecutor() = default;
    virtual void readExecutor(const std::string& command) = 0;
};