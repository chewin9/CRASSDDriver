#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include <tuple>
#include <string>

class Read {
public:
    const int MIN_INDEX = 0;
    const int MAX_INDEX = 100;
    const std::string INVALID_COMMAND = "INVALID COMMAND";

    void printInvalidCommand(void);
    std::string getSsdOutputData(void);
    
    std::tuple<std::string, std::string> parse_command(const std::string& input);
};

class ShellRead : public Read, public IShellCommand{
public:
    ShellRead(IProcessExecutor* executor);
    bool Run(const std::string& input) override;

private:
    IProcessExecutor* executor_;
    
    void printResult(int index, std::string value);
    bool checkParameterValid(const std::string& input);

    const std::string ERROR_RETURN = "ERROR";
};