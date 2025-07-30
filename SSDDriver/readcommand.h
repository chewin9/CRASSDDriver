#pragma once
#include <string>
class ReadCommand {
 public:
  bool Execute(int lba, std::string value);
};