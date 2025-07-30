#pragma once
#include <string>
#include "process_executor.h"
#include <tuple>
class ShellRead {
public:
    ShellRead(IProcessExecutor* executor);
    std::string read(const std::string input);
    std::string fullRead(const std::string input);
    
private:
    IProcessExecutor* executor_;
    std::string getSSDOutputDataWithIndex(const std::string& data, int index);
    std::string getSsdOutputData(void);
    std::tuple<std::string, std::string> parse_command(const std::string& input);
    std::string appendSSDData(void);
    bool isInvalidIndex(int index);
    const std::string NO_SSD_DATA = "0x00000000";
    const std::string ERROR_RETURN = "ERROR";
    const std::string READ_COMMAND = "read";
    const std::string FULLREAD_COMMAND = "fullread";
    const std::string TEMP_FILE_NAME = "temp_fullread.txt";
    const int MIN_INDEX = 0;
    const int MAX_INDEX = 100;
};