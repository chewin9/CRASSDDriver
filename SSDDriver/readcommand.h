#pragma once
#include <string>
class ReadCommand {
 private:
  const int MIN_VAL = 0;
  const int MAX_VAL = 99;
  const int SIZE_OF_HEX_NOTATION = 2;
  const int SIZE_OF_VALUE = 10;
  bool IsValidParameter(int lba, std::string value);
  bool IsValidLba(int lba);
  bool IsValidValue(std::string value);
 public:
  bool Execute(int lba, std::string value);
};