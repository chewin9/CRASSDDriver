#include "gmock/gmock.h"
#include "file_io.h"
#include "command_parser.h"
#include <fstream>
#include <cstdio>
#include <string>

using namespace::testing;

class FileIOFixture : public Test {
public:

    FileIO file_io;
    ParsedCommand pc;
};

TEST_F(FileIOFixture, WriteErrorOutput) {

    std::remove("ssd_output.txt");
    file_io.WriteValueToOutputFile("ERROR");

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


TEST_F(FileIOFixture, WriteOutputWithError) {

    std::remove("ssd_output.txt");
    pc.errorFlag = true;
    file_io.WriteOutput(pc);

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
