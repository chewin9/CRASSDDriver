#include "gmock/gmock.h"
#include "file_io.h"
#include <fstream>
#include <cstdio>
#include <string>

using namespace::testing;

class FileIOFixture : public Test {
public:

    FileIO file_io;

    const int MAX_VAL_SIZE = 100;
    std::string normalValue = "0xFFFFFFFF";
    std::vector<std::string> abnormalValue = { "0xFFF", "0x00000GGG", "FFFFFFFF",
                                              "" };
};

TEST_F(FileIOFixture, WriteErrorOutput) {

    std::remove("ssd_output.txt");

    file_io.WriteErrorOutput();

    std::ifstream inFile("ssd_output.txt");
    bool fileExists = inFile.is_open();

    std::string line;
    if (fileExists) {
        std::getline(inFile, line);
        inFile.close();
    }

    EXPECT_EQ(true, fileExists);
    EXPECT_EQ(line, "ERROR");
}
