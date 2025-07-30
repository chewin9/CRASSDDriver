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
    const std::string SSD_NAND_FILE = "ssd_nand.txt";
    const std::string SSD_OUTPUT_FILE = "ssd_output.txt";

    bool OpenInput();
    std::vector<std::pair<int, std::string>> getEntriesFromInput(ParsedCommand pc);
    void WriteOutput(std::vector<std::pair<int, std::string>> entries);
    bool WriteErrorOutput();
    void CloseInput();

    bool OpenOutput(std::string file);
    void CloseOutput();

    ~FileIO();
};