#pragma once
#include <string>
class ReadCommand {
 public:
  bool Read(int lba, std::string value);
};