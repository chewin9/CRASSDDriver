#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "command_parser.h"

class FileIO {
public:

    std::ifstream m_input;
    std::ofstream m_output;
    void WriteValueToOutputFile(std::string str);
    bool OpenInput();
    std::vector<std::pair<int, std::string>> getEntriesFromInput(ParsedCommand& pc);
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