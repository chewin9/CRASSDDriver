#pragma once
#include <string>
struct ParsedCommand {
  std::string opCode = "";
  int lba = -1;
  std::string value = "";
  bool errorFlag = false;
};

class CommandParser {
 private:
  void CheckException(int argc, char* argv[], const ParsedCommand& cmd);
  bool IsLbaErrorExist(const ParsedCommand& cmd);
  bool IsValueErrorExist(const ParsedCommand& cmd);

 public:
  static constexpr int MIN_VAL = 0;
  static constexpr int MAX_VAL = 99;
  static constexpr int SIZE_OF_HEX_NOTATION = 2;
  static constexpr int SIZE_OF_VALUE = 10;

  ParsedCommand ParseCommand(int argc, char* argv[]);
};
