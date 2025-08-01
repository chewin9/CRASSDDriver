#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include "shell.h"
#include "command_invoker.h"
#include "shell_util.h"

std::tuple<std::string, std::string> Shell::parse_command(const std::string& input) {
    std::istringstream iss(input);
    std::string cmd;
    std::string num_str;
    iss >> cmd >> num_str;
    return { cmd, num_str };
}

std::string Shell::get_command(std::string& input) {
    std::cout << "> "; // Prompt
    std::getline(std::cin, input);

    std::tuple<std::string, std::string> parseCommand = parse_command(input);
    return std::get<0>(parseCommand);
}

int Shell::Run(std::string arg) {
    std::string input;
    CommandInvoker invoker;

    invoker.RegisterAllCommand(Get_Executor());

    if (!arg.empty()) {
        script_runner->ScriptRunnerMode(arg, &testscriptfile);
        return 0;
    }

    while (true) {
        std::string cmd = get_command(input);

        if (cmd == "exit") {
            std::cout << "Exiting program.\n";
            break;
        }

        if (invoker.executeCommand(cmd, input)) {
            continue;
        }

        if (nullptr == script_runner->getScript(input)) {
            Util::printInvalidCommand();
            std::cout << "You typed : " << input << std::endl;
            continue;
        }

        if (script_runner->runScript(input)) {
            continue;
        }
    }

    return 0;
}