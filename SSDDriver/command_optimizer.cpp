#include "command_optimizer.h"
#include <algorithm>
void WriteCommandOptimizer::Optimize(const ParsedCommand& cmdInfo, list<ParsedCommand>& bufferList) {
    ParsedCommand cmd = cmdInfo;
    DivideWriteAndEraseBuffer(bufferList);
    OptimizeWriteCommand(cmd, bufferList);
    MergeWriteAndEraseBuffer(cmdInfo, bufferList);
}

void WriteCommandOptimizer::DivideWriteAndEraseBuffer(list<ParsedCommand>& bufferList) {
    for (auto i : bufferList) {
        if (i.opCode == WRITE_OPCODE)
            writeCommandList.push_back(i);
        else if (i.opCode == ERASE_OPCODE)
            eraseCommandList.push_back(i);
    }
}

void WriteCommandOptimizer::MergeWriteAndEraseBuffer(ParsedCommand cmdInfo, list<ParsedCommand>& bufferList) {
    eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
    bufferList = eraseCommandList;
}

void WriteCommandOptimizer::OptimizeWriteCommand(ParsedCommand& cmdInfo, list<ParsedCommand>& bufferList) {
    for (auto it = writeCommandList.begin(); it != writeCommandList.end();) {
        if (it->lba == cmdInfo.lba) {
            it->value = cmdInfo.value;
            eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
            bufferList = eraseCommandList;
            return;
        }
        ++it;
    }
    writeCommandList.push_back(cmdInfo);
}

void EraseCommandOptimizer::Optimize(const ParsedCommand& cmdInfo, list<ParsedCommand>& bufferList) {
    DivideWriteAndEraseBuffer(bufferList);
    OptimizeEraseCommand(cmdInfo);
    MergeWriteAndEraseBuffer(cmdInfo, bufferList);
}

void EraseCommandOptimizer::MergeWriteAndEraseBuffer(ParsedCommand cmdInfo, list<ParsedCommand>& bufferList) {
    eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
    bufferList = eraseCommandList;
}

void EraseCommandOptimizer::OptimizeEraseCommand(ParsedCommand cmdInfo) {
    int mergedStart = cmdInfo.lba;
    int mergedEnd = cmdInfo.lba + cmdInfo.erase_size - 1;

    IgnoreWrite(mergedStart, mergedEnd);
    bool merged = MergeErase(mergedStart, mergedEnd);

    if (merged) {
        RearrangeMergedErase(mergedStart, mergedEnd);
        return;
    }

    eraseCommandList.push_back(cmdInfo);
}



void EraseCommandOptimizer::IgnoreWrite(int& mergedStart, int& mergedEnd) {
    for (auto it = writeCommandList.begin(); it != writeCommandList.end();) {
        int writeLba = it->lba;
        if (writeLba >= mergedStart && writeLba <= mergedEnd) {
            it = writeCommandList.erase(it);
        }
        else {
            ++it;
        }
    }
}


bool EraseCommandOptimizer::MergeErase(int& mergedStart, int& mergedEnd) {
    bool flag = false;
    for (auto it = eraseCommandList.begin(); it != eraseCommandList.end();) {
        if ((it->lba == mergedEnd + 1 || it->lba + it->erase_size == mergedStart ||
            (it->lba <= mergedStart &&
                it->lba + it->erase_size - 1 >= mergedStart) ||
            (it->lba <= mergedEnd && it->lba + it->erase_size - 1 >= mergedEnd)) ||
            (mergedStart <= it->lba && it->lba <= mergedEnd &&
                mergedStart <= it->lba + it->erase_size - 1 &&
                it->lba + it->erase_size - 1 <= mergedEnd)) {
            mergedStart = std::min(mergedStart, it->lba);
            mergedEnd = std::max(mergedEnd, it->lba + it->erase_size - 1);
            it = eraseCommandList.erase(it);
            flag = true;
        }

        else {
            ++it;
        }
    }

    return flag;
}


void EraseCommandOptimizer::RearrangeMergedErase(int& mergedStart, int& mergedEnd) {
    int curStart = mergedStart;
    while (curStart <= mergedEnd) {
        int curEnd = std::min(curStart + MAX_RANGE - 1, mergedEnd);
        eraseCommandList.push_back(
            { "E", curStart, "", false, curEnd - curStart + 1 });
        curStart = curEnd + 1;
    }
}

void EraseCommandOptimizer::DivideWriteAndEraseBuffer(list<ParsedCommand>& bufferList) {
    for (auto i : bufferList) {
        if (i.opCode == WRITE_OPCODE)
            writeCommandList.push_back(i);
        else if (i.opCode == ERASE_OPCODE)
            eraseCommandList.push_back(i);
    }
}