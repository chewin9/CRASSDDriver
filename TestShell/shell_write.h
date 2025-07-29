#pragma once
#include "iprocess_executor.h"
#include <memory>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class ShellWrite {
public:
    ShellWrite(IProcessExecutor* executor) : executor_(executor) {}


    std::vector<std::string> splitBySpace(const std::string& str) {
        std::istringstream iss(str);
        std::vector<std::string> tokens;
        std::string word;
        while (iss >> word) {
            tokens.push_back(word);
        }
        return tokens;
    }

    void IssueWrite(const std::string& input) {
        
        // check input parameter valid
        //"write 3 0xAAAABBBB"   
        vector<string> str;
        std::string word;
        std::istringstream iss(input);
        while (iss >> word) { // each word separated by space
            str.push_back(word);
        }
        if (input.find("write ") == 0) {
            std::string cmdLine = "ssd.exe W " + input.substr(6);
            executor_->Process(cmdLine);
        }
    }

private:
    IProcessExecutor* executor_;
};