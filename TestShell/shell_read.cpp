#include "shell_read.h"
#include <fstream>
#include <sstream>
#include <iostream>

ShellRead::ShellRead(IProcessExecutor* executor) : executor_(executor) {}

std::string ShellRead::read(const std::string input) {
    // Parse �ϴ� �κ�
    std::tuple<std::string, std::string> parseCommand = parse_command(input);

    //SSD ����
    if (std::get<0>(parseCommand) == "READ") {
        std::string cmdLine = "ssd.exe R " + std::get<1>(parseCommand);
        executor_->Process(cmdLine);
    }
    //��� �� �б�
    std::string fullSSDData = getSSDData();

    //��ü ������� index �� ã��
    std::string ret;
    int index = std::stoi(std::get<1>(parseCommand));
    ret = getSSDDataWithIndex(fullSSDData, index);

    return ret;
}


std::string ShellRead::getSSDDataWithIndex(const std::string& data, int index) {
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
    return NO_SSD_DATA; // �ε����� �� ã���� ��
}

std::tuple<std::string, std::string> ShellRead::parse_command(const std::string& input) {
    std::istringstream iss(input);
    std::string cmd;
    std::string num_str;
    iss >> cmd >> num_str;
    return { cmd, num_str };
}

std::string ShellRead::getSSDData(void) {
    
    std::ifstream file("ssd_output.txt");
    if (!file.is_open()) {
        std::cout << "open fail\n";
        return "";
    }
    std::string ret((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    return ret;
}