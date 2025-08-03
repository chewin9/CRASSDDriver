#pragma once
#include <string>

#include "command_buffer.h"
#include "command_parser.h"
#include "IFileIO.h"

class SsdOperationHandler {
 private:
  std::unordered_map<int, std::string> nandData;
  IFileIO& fileHandler;
  CommandBuffer& cmdBuffer;
  void UpdateData(const ParsedCommand& cmdInfo);
  std::string ReadData(const ParsedCommand& cmdInfo);
  void EraseData(const ParsedCommand& cmdInfo);
  void WriteToNand(const ParsedCommand& cmdInfo);
  void EraseFromNand(const ParsedCommand& cmdInfo);
  std::string ReadFromNand(const ParsedCommand& cmdInfo);
  void FlushToNand(std::list<ParsedCommand>& bufferList);
  bool IsErrorExist(const ParsedCommand& cmdInfo);
 public:
  SsdOperationHandler(IFileIO& handler, CommandBuffer& cmdBuffer)
      : fileHandler{handler}, cmdBuffer{cmdBuffer} {}

  void WriteAndErase(const ParsedCommand& cmdInfo);
  void Read(const ParsedCommand& cmdInfo);
  void Flush();
};