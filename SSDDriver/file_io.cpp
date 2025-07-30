#include "file_io.h"
#include <iostream>
#include <vector>


bool FileIO::WriteErrorOutput() {

    if (!OpenOutput(SSD_OUTPUT_FILE)) return false;

    m_output << "ERROR" << "\n";
    CloseOutput();
    return true;
}

bool FileIO::OpenInput() {
    m_input.open(SSD_NAND_FILE);
    if (!m_input.is_open()) return false;
}

std::vector<std::pair<int, std::string>> FileIO::getEntriesFromInput(ParsedCommand pc) { 
    OpenInput();

    std::vector<std::pair<int, std::string>> entries;
    bool updated = false;

    std::string line;
    while (std::getline(m_input, line)) {
        std::istringstream iss(line);

        int existing_LBA;
        std::string existing_value;

        if (iss >> existing_LBA >> existing_value) {
            if (existing_LBA == pc.lba) {

                entries.emplace_back(pc.lba, pc.value);
                updated = true;
            }
            else {
                entries.emplace_back(existing_LBA, existing_value);
            }
        }
    }

    if (!updated) {
        entries.emplace_back(pc.lba, pc.value);
    }

    CloseInput();

    return entries;
}

void FileIO::CloseInput() {
    if (m_input.is_open()) {
        m_input.close();
    }
}


void FileIO::WriteOutput(ParsedCommand pc) {

    if (pc.errorFlag) {
        WriteErrorOutput();
        return;
    }
       
    std::vector<std::pair<int, std::string>> entries =  getEntriesFromInput(pc);

    OpenOutput(SSD_NAND_FILE);
    for (const auto& entry : entries) {
        m_output << entry.first << " " << entry.second << "\n";
    }
    CloseOutput();
}


bool FileIO::OpenOutput(std::string file) {

    m_output.open(file, std::ios::out | std::ios::trunc);
    if (!m_output.is_open()) return false;
    return true;
}

void FileIO::CloseOutput() {
    if (m_output.is_open()) {
        m_output.close();
    }
}

FileIO::~FileIO() {
}