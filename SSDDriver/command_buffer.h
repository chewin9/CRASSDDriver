#pragma once
#include "command_parser.h"
#include <string>
#include <unordered_map>
#include <list>
class CommandBuffer {
 private:
  std::vector<std::string> ConvertParsedCommandToStringList(const std::list<ParsedCommand>& cmdList);
  std::list<ParsedCommand> ParsingStringtoBuf(std::vector<std::string>& bufferList);
  void OptimizeBuffer(std::list<ParsedCommand>& bufferList,const ParsedCommand& cmd);
 public:
  void AddBuffer(const ParsedCommand& cmdInfo);
  std::string ReadBuffer(const ParsedCommand& cmdInfo);

  std::unordered_map<int, std::string> GetBufferToFlush();
};