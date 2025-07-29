#pragma once
#include "iprocess_executor.h"
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

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

    bool is_valid_unsigned(const std::string& str) {
        size_t idx = 0;
        try {
            std::stoul(str, &idx, 0); // base 0: auto-detect base (hex, dec, oct)
            return idx == str.size(); // true if entire string was used
        }
        catch (const std::invalid_argument&) {
            return false; // Not a number at all
        }
        catch (const std::out_of_range&) {
            return false; // Number outside unsigned long range
        }
    }

    void printError() {
        std::cout << "INVALID_COMMAND" << std::endl;
    }

    void IssueWrite(const std::string& input) {
        
        // check input parameter valid
        //"write 3 0xAAAABBBB"   
        vector<string> cmdLineVector = splitBySpace(input);

        if (cmdLineVector[0] != "write") {
            printError();
            return; // not expected command
        }
        
        int cmdLba = std::stoi(cmdLineVector[1]);
        if (!((0 <= cmdLba) && (cmdLba < 100))) {
            printError();
            return;  // not valid command 
        }

        if (input.find("write ") == 0) {
            std::string cmdLine = "ssd.exe W " + input.substr(6);
            executor_->Process(cmdLine);
        }
    }

private:
    IProcessExecutor* executor_;
};