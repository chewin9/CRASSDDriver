#include "writecommand.h"
#include "file_io.h"

#include <iostream>
#include <vector>

bool WriteCommand::Execute() {
  int LBA = cmdInfo.lba;
  std::string value = cmdInfo.value;

  std::vector<std::pair<int, std::string>> entries = file_io.getEntriesFronInput(cmdInfo);
  file_io.WriteOutput(entries);

  return true;
};