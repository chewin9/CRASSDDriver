#pragma once
#include "iprocess_executor.h"
#include <vector>

using namespace std;

class ShellWrite {
public:
    ShellWrite(IProcessExecutor* executor) : executor_(executor) {}

    void IssueWrite(const std::string& input);

private:
    void printError();

    bool checkParameterValid(const std::string& input);

    std::vector<std::string> splitBySpace(const std::string& str);

    bool convertStoI(const std::string& str, int& val);

    bool is_valid_unsigned(const std::string& str);

    IProcessExecutor* executor_;
};