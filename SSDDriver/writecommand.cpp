#include "writecommand.h"
#include "file_io.h"
#include <iostream>
#include <vector>

bool WriteCommand::Execute(int LBA, std::string value) {

    std::ifstream input(ICommand::SSD_NAND_FILE);
    std::vector<std::pair<int, std::string>> entries;
    bool updated = false;

    if (input.is_open()) {
        std::string line;
        while (std::getline(input, line)) {
            std::istringstream iss(line);
            int existing_LBA;
            std::string existing_value;
            if (iss >> existing_LBA >> existing_value) {
                if (existing_LBA == LBA) {
                    entries.emplace_back(LBA, value);
                    updated = true;
                }
                else {
                    entries.emplace_back(existing_LBA, existing_value);
                }
            }
        }
        input.close();
    }

    if (!updated) {
        entries.emplace_back(LBA, value);
    }

    std::ofstream output(SSD_NAND_FILE, std::ios::out | std::ios::trunc);
    if (!output.is_open()) return false;

    for (const auto& entry : entries) {
        output << entry.first << " " << entry.second << "\n";
    }

    output.close();
    return true;

};

bool WriteCommand::Execute() {
    std::cerr << "Execute() without parameters is not supported in WriteCommand.\n";
    return false;
}