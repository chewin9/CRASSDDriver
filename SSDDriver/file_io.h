#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "command_parser.h"

class FileIO {
public:

    std::ifstream m_input;
    std::ofstream m_output;
    void WriteValueToOutputFile(std::string str);
    bool OpenInput();
    std::unordered_map<int, std::string> LoadDataFromInput();
    bool UpdateData(std::unordered_map<int, std::string>& entries,
        const ParsedCommand& pc);
    std::unordered_map<int, std::string> getEntriesFromInput(ParsedCommand& pc);
    void WriteOutput(ParsedCommand pc);
    void CloseInput();
    bool ReadNandFile(ParsedCommand& pc);
    bool OpenOutput(std::string file);
    void CloseOutput();

    ~FileIO();

private:
    const std::string SSD_NAND_FILE = "ssd_nand.txt";
    const std::string SSD_OUTPUT_FILE = "ssd_output.txt";
};