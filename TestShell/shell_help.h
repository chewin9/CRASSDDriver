#pragma once
#include <string>
#include "process_executor.h"
#include <tuple>
class ShellHelp {
public:
    ShellHelp(IProcessExecutor* executor);
    void help(const std::string input);
    
private:
    IProcessExecutor* executor_;
    std::tuple<std::string, std::string> parse_command(const std::string& input);
    void crewIntroduce(void);
    void helpCommand(void);
    void writeCommand(void);
    void readCommand(void);
    void exitCommand(void);
    void fullwriteCommand(void);
    void fullreadCommand(void);
};