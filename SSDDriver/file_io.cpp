#include "file_io.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::unordered_map<int, std::string> FileIO::LoadDataFromInput() {
  std::ifstream input(SSD_NAND_FILE);
  if (!input.is_open()) {
    return {};
  }
  std::unordered_map<int, std::string> entries;
  std::string line;
  while (std::getline(input, line)) {
    std::istringstream iss(line);
    int existingLBA;
    std::string existingValue;
    if (iss >> existingLBA >> existingValue) {
      entries[existingLBA] = std::move(existingValue);
    }
  }
  return entries;
}

void FileIO::SaveData(std::unordered_map<int, std::string> entries) {
  std::ofstream output(
      SSD_NAND_FILE, std::ios::out | std::ios::trunc);
  if (!output.is_open()) {
    return;
  }
  for (const auto& entry : entries) {
    output << entry.first << " " << entry.second << "\n";
  }
}

void FileIO::WriteValueToOutputFile(std::string val) {
  std::ofstream outFile(SSD_OUTPUT_FILE);
  if (!outFile.is_open()) {
    return;
  }
  outFile << val << std::endl;
}

