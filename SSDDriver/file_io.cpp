#include "file_io.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

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

void FileIO::GenFolderAndEmtyFiles() {

    if (fs::exists(SSD_COMMAND_BUFFER_FOLDER)) return;
    
    fs::create_directory(SSD_COMMAND_BUFFER_FOLDER);
    fs::path dir{ SSD_COMMAND_BUFFER_FOLDER };

    for (int i = 1; i <= 5; ++i) {
        fs::path filePath = dir / (std::to_string(i) + "_empty");
        std::ofstream ofs(filePath);
    }

}

void FileIO::EraseFolder() {
    fs::path dir{ SSD_COMMAND_BUFFER_FOLDER };
    std::error_code ec;

    if (fs::exists(dir, ec) && fs::is_directory(dir, ec)) {
        fs::remove_all(dir, ec);
    }

}