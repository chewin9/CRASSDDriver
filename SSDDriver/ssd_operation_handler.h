#pragma once
#include "file_io.h"
#include "command_parser.h"
#include "command_buffer.h"
#include <string>
class SsdOperationHandler {
 private:
  std::unordered_map<int, std::string> nandData;
  FileIO& fileHandler;
  CommandBuffer& cmdBuffer;
  void UpdateData(const ParsedCommand& cmdInfo);
  std::string ReadData(const ParsedCommand& cmdInfo);
  bool EraseData(const ParsedCommand& cmdInfo);
  void _Write(const ParsedCommand& cmdInfo);
  void _Erase(const ParsedCommand& cmdInfo);
 public:
  SsdOperationHandler(FileIO& handler, CommandBuffer& cmdBuffer)
      : fileHandler{handler}, cmdBuffer{cmdBuffer} {}

  void Write(const ParsedCommand& cmdInfo);
  bool Read(const ParsedCommand& cmdInfo);
  bool Erase(const ParsedCommand& cmdInfo);
  void Flush();
  // bool erase();
};