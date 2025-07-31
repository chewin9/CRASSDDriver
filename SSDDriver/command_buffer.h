#pragma once
#include "command_parser.h"
#include <string>
#include <unordered_map>
class CommandBuffer {
 public:
  void AddBuffer(const ParsedCommand& cmdInfo);
  std::string ReadBuffer(const ParsedCommand& cmdInfo);
  std::unordered_map<int, std::string> GetBufferToFlush();
};