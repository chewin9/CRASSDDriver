#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "command_parser.h"
#include "IFileIO.h"

using std::list;
using std::string;
using std::vector;

class CommandBuffer {
 private:
  IFileIO& fileio;
  list<ParsedCommand> writeCommandList;
  list<ParsedCommand> eraseCommandList;
  list<ParsedCommand> bufferList;

  vector<string> ParsingBuftoString(const list<ParsedCommand>& cmdList);
  list<ParsedCommand> ParsingStringtoBuf(const vector<string>& bufferArr);

  void OptimizeBuffer(const ParsedCommand& cmd);
  void ConvertWriteZeroValToErase(ParsedCommand& cmd);
  void DivideWriteAndEraseBuffer();
  void MergeWriteAndEraseBuffer(ParsedCommand cmdInfo);
  void OptimizeWriteCommand(ParsedCommand& cmdInfo);
  void OptimizeEraseCommand(ParsedCommand cmdInfo);
  void InitializeBuffer();
  void IgnoreWrite(int& mergedStart, int& mergedEnd);
  bool MergeErase(int& mergedStart, int& mergedEnd);
  void RearrangeMergedErase(int& mergedStart, int& mergedEnd);

 public:
  const std::string ERASED_VALUE = "0x00000000";
  const std::string VALUE_NOT_FIND = "";
  const std::string WRITE_OPCODE = "W";
  const std::string ERASE_OPCODE = "E";
  const int MAX_RANGE = 10;
  vector<string> RegisterBuffer(const ParsedCommand& cmdInfo,
                                const vector<string>& currentBuffer);
  string ReadBuffer(const ParsedCommand& cmdInfo,
                    const vector<string>& currentBuffer);
  bool IsFlushNeeded(const vector<string>& currentBuffer);
  list<ParsedCommand> GetCommandBuffer(const vector<string>& currentBuffer);
};