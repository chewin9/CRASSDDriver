#pragma once

#include "command_parser.h"
#include <list>
using std::list;

class ICommandOptimizer {
public:
    virtual ~ICommandOptimizer() = default;
    virtual void Optimize(const ParsedCommand& cmdInfo, list<ParsedCommand>& bufferList) = 0;
};

class WriteCommandOptimizer : public ICommandOptimizer {
    list<ParsedCommand> writeCommandList;
    list<ParsedCommand> eraseCommandList;
    const std::string ERASED_VALUE = "0x00000000";
    const std::string VALUE_NOT_FIND = "";
    const std::string WRITE_OPCODE = "W";
    const std::string ERASE_OPCODE = "E";

public:
    void Optimize(const ParsedCommand& cmdInfo, list<ParsedCommand>& bufferList) override;
    void DivideWriteAndEraseBuffer(list<ParsedCommand>& bufferList);
    void OptimizeWriteCommand(ParsedCommand& cmdInfo, list<ParsedCommand>& bufferList);
    void MergeWriteAndEraseBuffer(ParsedCommand cmdInfo, list<ParsedCommand>& bufferList);
};

class EraseCommandOptimizer : public ICommandOptimizer {
    list<ParsedCommand> writeCommandList;
    list<ParsedCommand> eraseCommandList;
    const int MAX_RANGE = 10;
    const std::string ERASED_VALUE = "0x00000000";
    const std::string VALUE_NOT_FIND = "";
    const std::string WRITE_OPCODE = "W";
    const std::string ERASE_OPCODE = "E";
public:
    void DivideWriteAndEraseBuffer(list<ParsedCommand>& bufferList);
    void Optimize(const ParsedCommand& cmdInfo, list<ParsedCommand>& bufferList) override;
    void RearrangeMergedErase(int& mergedStart, int& mergedEnd);
    bool MergeErase(int& mergedStart, int& mergedEnd);
    void IgnoreWrite(int& mergedStart, int& mergedEnd);
    void OptimizeEraseCommand(ParsedCommand cmdInfo);
    void MergeWriteAndEraseBuffer(ParsedCommand cmdInfo, list<ParsedCommand>& bufferList);
};