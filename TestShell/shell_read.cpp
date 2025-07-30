#include "shell_read.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string ShellRead::read(int index) {
    std::ifstream file("ssd_output.txt");
    if (!file.is_open()) {
        std::cout << "open fail\n";
        return "";
    }
    std::string outputData((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    return findOutput(outputData, index);
}

std::string ShellRead::findOutput(const std::string& data, int index) {
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream line_stream(line);
        int idx;
        std::string value;
        if (line_stream >> idx >> value) {
            if (idx == index) {
                return value;
            }
        }
    }
    return ""; // 인덱스를 못 찾았을 때
}