#pragma once
#include <string>
#include "process_executor.h"
#include "ishell_command.h"
#include <vector>

class ShellHelp : public IShellCommand {
public:
    ShellHelp(IProcessExecutor* executor);
    bool Run(const std::string &input) override;
    
private:
    IProcessExecutor* executor_;
    std::vector<std::string> splitBySpace(const std::string& cmd);
    void crewIntroduce(void);
    void helpCommand(void);
    void writeCommand(void);
    void readCommand(void);
    void exitCommand(void);
    void fullwriteCommand(void);
    void fullreadCommand(void);
    void eraseCommand(void);
    void eraserangeCommand(void);
    void flushCommand(void);
};