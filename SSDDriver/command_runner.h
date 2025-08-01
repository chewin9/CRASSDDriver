#pragma once

#include "command_buffer.h"
#include "command_factory.h"
#include "command_parser.h"
#include "file_io.h"
#include "ssd_operation_handler.h"

class CommandRunner {
 public:
  CommandRunner();
  ~CommandRunner();

  void Run(int argc, char* argv[]);

 private:
  CommandParser parser;
  FileIO fileio;
  CommandBuffer cmdBuffer;
  SsdOperationHandler* opHandler = nullptr;
};
