#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include <vector>

using namespace std;

class Write {
public:
    const int START_LBA = 0;
    const int END_LBA = 100;
    const std::string INVALID_PARAMETER = "INVALID PARAMETER";

    void printError();

    std::vector<std::string> splitBySpace(const std::string& str);

    bool convertStoI(const std::string& str, int& val);

    bool is_all_uppercase(const std::string& str);

    bool is_valid_unsigned(const std::string& str);
};

class ShellWrite : public Write , public IShellCommand {
public:
    ShellWrite(IProcessExecutor* executor) : executor_(executor) {}

    bool Run(const std::string& cmd) override;

private:

    bool checkParameterValid(const std::string& input);

    IProcessExecutor* executor_;
};