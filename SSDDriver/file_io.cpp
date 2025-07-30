#include "file_io.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool FileIO::OpenInput() {
  m_input.open(SSD_NAND_FILE);
  return m_input.is_open();
}

void FileIO::CloseInput() {
    if (m_input.is_open()) {
        m_input.close();
    }
}


std::vector<std::pair<int, std::string>> FileIO::getEntriesFromInput(
    ParsedCommand pc) {
  OpenInput();
  bool updated = false;
  std::vector<std::pair<int, std::string>> entries;

  std::string line;
  while (std::getline(m_input, line)) {
    std::istringstream iss(line);
    int existing_LBA;
    std::string existing_value;

    if (iss >> existing_LBA >> existing_value) {
        std::string valueToStore = (existing_LBA == pc.lba) ? pc.value : existing_value;
        entries.emplace_back(existing_LBA, valueToStore);

        if (existing_LBA == pc.lba) {
            updated = true;
        }
    }
  }

  if (!updated) {
    entries.emplace_back(pc.lba, pc.value);
  }

  CloseInput();
  return entries;
}


bool FileIO::OpenOutput(std::string file) {
  m_output.open(file, std::ios::out | std::ios::trunc);
  return m_output.is_open();
}

void FileIO::CloseOutput() {
  if (m_output.is_open()) {
    m_output.close();
  }
}

void FileIO::WriteOutput(ParsedCommand pc) {
  if (pc.errorFlag) {
    WriteValueToOutputFile("ERROR");
    return;
  }

  std::vector<std::pair<int, std::string>> entries = getEntriesFromInput(pc);
  OpenOutput(SSD_NAND_FILE);

  for (const auto& entry : entries) {
    m_output << entry.first << " " << entry.second << "\n";
  }

  CloseOutput();
}

void FileIO::WriteValueToOutputFile(std::string val) {
  std::ofstream outFile(SSD_OUTPUT_FILE);
  if (!outFile.is_open()) {
    return;
  }
  outFile << val << std::endl;
  outFile.close();
}

void FileIO::ReadNandFile(ParsedCommand pc) {
  if (pc.errorFlag) {
    WriteValueToOutputFile("ERROR");
    return;
  }

  OpenInput();
  std::string val = "0x00000000";
  std::string line;

  while (std::getline(m_input, line)) {
    std::istringstream iss(line);
    int existing_LBA;
    std::string existing_value;

    if (iss >> existing_LBA >> existing_value) {
      if (existing_LBA == pc.lba) {
        val = existing_value;
        break;
      }
    }
  }

  CloseInput();
  WriteValueToOutputFile(val);
}

FileIO::~FileIO() {}
