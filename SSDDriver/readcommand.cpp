#include "readcommand.h"

bool ReadCommand::Read(int lba, std::string val) { 
	if (lba < 0 || lba > 99) return false;
  if (val == "" || val.size() != 10 ||
      (val.size() > 2 && val.substr(0, 2) != "0x"))
    return false;
  for (int i = 2; i < 10; ++i) {
    char c = val[i];
    if (!isdigit(c) && !(c >= 'A' && c <= 'F')) {
      return false;
    }
  }
	return true; 
}