#pragma once
#include <string>
#include "process_executor.h"
#include <tuple>
class ShellRead {
public:
    ShellRead(IProcessExecutor* executor);
    void read(const std::string input);
    void fullRead(const std::string input);
    
private:
    IProcessExecutor* executor_;
    std::string getSsdOutputData(void);
    std::tuple<std::string, std::string> parse_command(const std::string& input);
    std::string appendSSDData(void);
    bool isInvalidIndex(int index);
    void printInvalidCommand(void);
    void printError(void);
    void runSSDDriver(std::string index);
    void printResult(int index, std::string value);
    

    const std::string NO_SSD_DATA = "0x00000000";
    const std::string ERROR_RETURN = "ERROR";
    const std::string READ_COMMAND = "read";
    const std::string FULLREAD_COMMAND = "fullread";
    const std::string TEMP_FILE_NAME = "temp_fullread.txt";
    const std::string INVALID_COMMAND= "INVALID COMMAND";
    const int MIN_INDEX = 0;
    const int MAX_INDEX = 100;
};