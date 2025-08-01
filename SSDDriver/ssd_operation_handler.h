#pragma once
#include <string>

#include "command_buffer.h"
#include "command_parser.h"
#include "file_io.h"
class SsdOperationHandler {
 private:
  std::unordered_map<int, std::string> nandData;
  FileIO& fileHandler;
  CommandBuffer& cmdBuffer;
  void UpdateData(const ParsedCommand& cmdInfo);
  std::string ReadData(const ParsedCommand& cmdInfo);
  void EraseData(const ParsedCommand& cmdInfo);
  void WriteToNand(const ParsedCommand& cmdInfo);
  void EraseFromNand(const ParsedCommand& cmdInfo);
  bool IsErrorExist(const ParsedCommand& cmdInfo);
  std::string ReadFromNand(const ParsedCommand& cmdInfo);
  void FlushToNand(std::list<ParsedCommand>& bufferList);

 public:
  SsdOperationHandler(FileIO& handler, CommandBuffer& cmdBuffer)
      : fileHandler{handler}, cmdBuffer{cmdBuffer} {}

  void Write(const ParsedCommand& cmdInfo);
  bool Read(const ParsedCommand& cmdInfo);
  bool Erase(const ParsedCommand& cmdInfo);
  void Flush();
  // bool erase();
};