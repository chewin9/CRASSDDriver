#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include "shell.h"
#include "shell_util.h"

std::tuple<std::string, std::string> Shell::parse_command(const std::string& input) {
    std::istringstream iss(input);
    std::string cmd;
    std::string num_str;
    iss >> cmd >> num_str;
    return { cmd, num_str };
}

string Shell::get_command(string& input) {
    std::cout << "> "; // Prompt
    std::getline(std::cin, input);

    std::tuple<std::string, std::string> parseCommand = parse_command(input);
    return std::get<0>(parseCommand);
}

int Shell::Run(std::string arg) {
    std::string input;

    if (!arg.empty()) {
        script_runner->ScriptRunnerMode(arg, &testscriptfile);
        return 0;
    }

    while (true) {
        string cmd = get_command(input);

        if (cmd == "exit") {
            std::cout << "Exiting program.\n";
            break;
        }
        else if (cmd == "write") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellWrite>(executor);
            cmd->Run(input);
        }
        else if (cmd == "fullwrite") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellFullWrite>(executor);
            cmd->Run(input);
        }
        else if (cmd == "read") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellRead>(executor);
            cmd->Run(input);
        }
        else if (cmd == "fullread") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellFullRead>(executor);
            cmd->Run(input);
        }
        else if (cmd == "flush") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellFlush>(executor);
            cmd->Run(input);
        }
        else if (cmd == "help") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellHelp>(executor);
            cmd->Run(input);
        }
        else if (cmd == "erase") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellErase>(executor);
            cmd->Run(input);
        }
        else if (cmd == "erase_range") {
            std::shared_ptr<IShellCommand> cmd = std::make_shared<ShellEraseRange>(executor);
            cmd->Run(input);
        }
        else {
            if (nullptr == script_runner->getScript(input)) {
                // no matching command
                Util::printInvalidCommand();
                std::cout << "You typed : " << input << std::endl;
                continue;
            }

            if (true == script_runner->runScript(input)) {
                std::cout << "script pass" << std::endl;
                continue;
            }

            std::cout << "script fail" << std::endl;
        }
    }
    return 0;
}
