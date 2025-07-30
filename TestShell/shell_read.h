#pragma once
#include <string>

class ShellRead {
public:
    ShellRead() = default;
    std::string read(int index);
private:
    std::string findOutput(const std::string& data, int index);
};