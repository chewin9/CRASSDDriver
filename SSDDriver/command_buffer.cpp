#include "command_buffer.h"
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
void CommandBuffer::OptimizeBuffer(std::list<ParsedCommand> &bufferList,
                                 const ParsedCommand &cmd) {
    ParsedCommand cmdInfo = cmd;
    if (cmdInfo.opCode == "W" && cmdInfo.value == "0x00000000")
    {
        cmdInfo.opCode = "E";
        cmdInfo.erase_size = 1;
        cmdInfo.value = "";
    }

    std::list<ParsedCommand> writeCommandList;
    std::list<ParsedCommand> eraseCommandList;
    for (auto i : bufferList)
    {
        if (i.opCode == "W")
            writeCommandList.push_back(i);
        else if (i.opCode == "E")
            eraseCommandList.push_back(i);
    }

    if (cmdInfo.opCode == "W")
    {
        for (auto it = writeCommandList.begin(); it != writeCommandList.end();)
        {
            if (it->lba == cmdInfo.lba)
            {
                it->value = cmdInfo.value;
                eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
                bufferList = eraseCommandList;
                return;
            }
            ++it;
        }

        writeCommandList.push_back(cmdInfo);

        eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
        bufferList = eraseCommandList;
        return;
    }

    int mergedStart = cmdInfo.lba;
    int mergedEnd = cmdInfo.lba + cmdInfo.erase_size - 1;
    bool merged = false;

    for (auto it = writeCommandList.begin(); it != writeCommandList.end();)
    {
        int writeLba = it->lba;
        if (writeLba >= mergedStart && writeLba <= mergedEnd)
        {
            it = writeCommandList.erase(it);
        }
        else
        {
            ++it;
        }
    }


    for (auto it = eraseCommandList.begin(); it != eraseCommandList.end();)
    {
        if (it->lba == mergedEnd - 1 || it->lba + it->erase_size == mergedStart ||
            (it->lba <= mergedStart && it->lba + it->erase_size - 1 >= mergedStart) ||
            (it->lba <= mergedEnd && it->lba + it->erase_size - 1 >= mergedEnd))
        {
            mergedStart = std::min(mergedStart, it->lba);
            mergedEnd = std::max(mergedEnd, it->lba + it->erase_size - 1);
            it = eraseCommandList.erase(it);
            merged = true;
        }
        else
        {
            ++it;
        }
    }

    const int MAX_RANGE = 10;

    if (merged)
    {
        int curStart = mergedStart;
        while (curStart <= mergedEnd)
        {
            int curEnd = std::min(curStart + MAX_RANGE - 1, mergedEnd);
            eraseCommandList.push_back({"E", curStart, "", false, curEnd - curStart + 1});
            curStart = curEnd + 1;
        }
    }
    else
    {
        eraseCommandList.push_back(cmdInfo);
    }

    eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
    bufferList = eraseCommandList;
}


void CommandBuffer::AddBuffer(const ParsedCommand &cmdInfo)
{
    std::vector<std::string> bufferArr;
    // std::vector<std::string> v = fileIo.GetBuffer();
    if (bufferArr.size() == 5)
    {
        // flush(bufferList);
    }
    std::list<ParsedCommand> bufferList = ParsingStringtoBuf(bufferArr);
    OptimizeBuffer(bufferList, cmdInfo);
    bufferArr = ConvertParsedCommandToStringList(bufferList);
}


std::string CommandBuffer::ReadBuffer(const ParsedCommand &cmdInfo)
{
    std::vector<std::string> bufferArr;

    std::list<ParsedCommand> bufferList = ParsingStringtoBuf(bufferArr);

    int readLba = cmdInfo.lba;
    for (auto rit = bufferList.rbegin(); rit != bufferList.rend(); ++rit)
    {
        if (rit->opCode == "W")
        {
            if (readLba == rit->lba)
            {
                return rit->value;
            }

            if (readLba >= rit->lba && readLba <= rit->lba + rit->erase_size - 1)
            {
                return "0x00000000";
            }
        }
    }

    // goto fileio and ask them for finding value
}


std::list<ParsedCommand> CommandBuffer::ParsingStringtoBuf(std::vector<std::string> &bufferList)
{
    std::list<ParsedCommand> parsedList;

    for (const auto &bufferStr : bufferList)
    {
        ParsedCommand cmd;
        std::istringstream ss(bufferStr);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, '_'))
        {
            tokens.push_back(token);
        }

        if (tokens.size() != 4)
        {
            cmd.errorFlag = true;
            parsedList.push_back(cmd);
            continue;
        }

        cmd.opCode = tokens[1];
        cmd.lba = std::stoi(tokens[2]);

        if (cmd.opCode == "W")
        {
            cmd.value = tokens[3];
        }
        else if (cmd.opCode == "E")
        {
            cmd.erase_size = std::stoi(tokens[3]);
        }
        else
        {
            cmd.errorFlag = true;
        }

        parsedList.push_back(cmd);
    }

    return parsedList;
}

std::vector<std::string> CommandBuffer::ConvertParsedCommandToStringList(
    const std::list<ParsedCommand> &cmdList) {
    std::vector<std::string> result;
    int index = 1;

    for (const auto &cmd : cmdList)
    {
        std::ostringstream oss;
        oss << index << "_" << cmd.opCode << "_" << cmd.lba << "_";

        if (cmd.opCode == "W")
        {
            oss << cmd.value;
        }
        else if (cmd.opCode == "E")
        {
            oss << cmd.erase_size;
        }
        else
        {
            oss << "INVALID";
        }

        result.push_back(oss.str());
        ++index;
    }

    return result;
}
