#pragma once
#include "iprocess_executor.h"
#include "ishell_command.h"
#include <tuple>
#include <string>
#include <vector>  
class Read {
public:
    const int MIN_INDEX = 0;
    const int MAX_INDEX = 100;
    const std::string SSD_OUTPUT_FILE = "ssd_output.txt";
    std::string getSsdOutputData(void);
    bool isDecimalString(const std::string& str);
    virtual void performReadToSSD(std::string index) = 0;

};

class ShellRead : public Read, public IShellCommand{
public:
    ShellRead(IProcessExecutor* executor);
    bool Run(const std::string& input) override;
    virtual void performReadToSSD(std::string index) override;
private:
    IProcessExecutor* executor_;
    
    void printResult(int index, std::string value);
    bool checkParameterValid(std::vector<std::string> commandVec);

    const std::string ERROR_RETURN = "ERROR";
    const int LBA_INDEX = 1;
    const int VALID_READ_COMMAND_SIZE = 2;
};