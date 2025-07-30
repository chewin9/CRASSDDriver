#include "ICommand.h"

bool ICommand::IsValidParameter(int lba, std::string val) {
    if (!IsValidLba(lba)) return false;
    if (!IsValidValue(val)) return false;

    return true;
}

bool ICommand::IsValidLba(int lba) {
    if (lba < MIN_VAL || lba > MAX_VAL) return false;

    return true;
}

bool ICommand::IsValidValue(std::string val) {
    if (val == "" || val.size() != SIZE_OF_VALUE ||
        (val.size() > SIZE_OF_HEX_NOTATION &&
            val.substr(0, SIZE_OF_HEX_NOTATION) != "0x")) {
        return false;
    }

    for (int i = SIZE_OF_HEX_NOTATION; i < SIZE_OF_VALUE; ++i) {
        char c = val[i];
        if (!isdigit(c) && !(c >= 'A' && c <= 'F')) {
            return false;
        }
    }
    return true;
}

bool ICommand::Execute(int lba, std::string val) {
    if (!IsValidParameter(lba, val)) {
        return false;
    }
    return true;
}
