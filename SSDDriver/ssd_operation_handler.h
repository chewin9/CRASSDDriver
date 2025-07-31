#pragma once
#include <vector>
#include "file_io.h"
class SsdOperationHandler {
 private:
  std::unordered_map<int, std::string> nandData;
  FileIO& fileHandler;
  ParsedCommand& cmdInfo;

 public:
  SsdOperationHandler(FileIO& handler, ParsedCommand& cmdInfo)
      : fileHandler{handler}, cmdInfo{cmdInfo} {}

  void write();

  bool read();
};