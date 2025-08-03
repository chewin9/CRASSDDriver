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
     const std::string ERASED_VALUE = "0x00000000";
     const std::string VALUE_NOT_FIND = "";
     const std::string WRITE_OPCODE = "W";
     const std::string ERASE_OPCODE = "E";

  void SelectOptimizer(const ParsedCommand& cmdInfo);
  void OptimizeBuffer(const ParsedCommand& cmdInfo);
  vector<string> RegisterBuffer(const ParsedCommand& cmdInfo,
                                const vector<string>& currentBuffer);
  string ReadBuffer(const ParsedCommand& cmdInfo,
                    const vector<string>& currentBuffer);
  bool IsFlushNeeded(const vector<string>& currentBuffer);
  list<ParsedCommand> GetCommandBuffer(const vector<string>& currentBuffer);
};