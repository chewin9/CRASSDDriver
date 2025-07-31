#pragma once
#include <string>
#include "process_executor.h"
#include "ishell_command.h"
#include <tuple>

class ShellHelp : public IShellCommand {
public:
    ShellHelp(IProcessExecutor* executor);
    //virtual bool Run(const std::string & cmd) = 0;
    bool Run(const std::string &input) override;
    
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