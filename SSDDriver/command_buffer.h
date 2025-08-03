#pragma once
#include <list>
#include <string>
#include <unordered_map>

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
  list<ParsedCommand> ParsingStringtoBuf(vector<string>& bufferList);
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

  CommandBuffer(IFileIO& fileio) : fileio{fileio} {}
  void RegisterBuffer(const ParsedCommand& cmdInfo);
  string ReadBuffer(const ParsedCommand& cmdInfo);
  bool IsFlushNeeded();
  list<ParsedCommand> GetCommandBuffer();
};