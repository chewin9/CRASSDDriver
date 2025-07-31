#pragma once
#include "file_io.h"
#include "command_parser.h"
#include <string>
class SsdOperationHandler {
 private:
  std::unordered_map<int, std::string> nandData;
  FileIO& fileHandler;
  void UpdateData(const ParsedCommand& cmdInfo);
  std::string ReadData(const ParsedCommand& cmdInfo);
  bool EraseData(const ParsedCommand& cmdInfo);

 public:
  SsdOperationHandler(FileIO& handler) : fileHandler{handler}{}

  void Write(const ParsedCommand& cmdInfo);
  bool Read(const ParsedCommand& cmdInfo);
  bool Erase(const ParsedCommand& cmdInfo);
  // bool erase();
};