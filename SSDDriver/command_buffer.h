#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "command_parser.h"
#include "IFileIO.h"
#include "command_optimizer.h"
using std::list;
using std::string;
using std::vector;

class CommandBuffer {
 private:
  list<ParsedCommand> bufferList;
  std::unique_ptr<ICommandOptimizer> optimizer;

  vector<string> ParsingBuftoString(const list<ParsedCommand>& cmdList);
  list<ParsedCommand> ParsingStringtoBuf(const vector<string>& bufferArr);
  void ConvertWriteZeroValToErase(ParsedCommand& cmd);
  void InitializeBuffer(const vector<string> &currentBuffer);

 public:
     static constexpr const char* ERASED_VALUE = "0x00000000";
     static constexpr const char* VALUE_NOT_FIND = "";
     static constexpr const char* WRITE_OPCODE = "W";
     static constexpr const char* ERASE_OPCODE = "E";
     CommandBuffer(std::unique_ptr<ICommandOptimizer> optimizer);
  void OptimizeBuffer(const ParsedCommand& cmdInfo);
  vector<string> RegisterBuffer(const ParsedCommand& cmdInfo,
                                const vector<string>& currentBuffer);
  string ReadBuffer(const ParsedCommand& cmdInfo,
                    const vector<string>& currentBuffer);
  bool IsFlushNeeded(const vector<string>& currentBuffer);
  list<ParsedCommand> GetCommandBuffer(const vector<string>& currentBuffer);
};