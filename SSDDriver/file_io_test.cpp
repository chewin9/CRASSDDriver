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

TEST_F(FileIOFixture, WriteOutputWithNewLBA) {
    std::remove("ssd_nand.txt");
    pc = { "W", 3, "0xABCDEF01", false };
    file_io.WriteOutput(pc);

    std::ifstream inFile("ssd_nand.txt");
    ASSERT_TRUE(inFile.is_open());

    std::string line;
    std::getline(inFile, line);
    inFile.close();

    EXPECT_EQ(line, "3 0xABCDEF01");
}

TEST_F(FileIOFixture, WriteOutput_OverwritesExistingLBA) {

    std::ofstream out("ssd_nand.txt");
    out << "5 0xAAAA0000\n";
    out.close();

    pc = { "W", 5, "0xBBBB1111", false };
    file_io.WriteOutput(pc);

    std::ifstream inFile("ssd_nand.txt");
    ASSERT_TRUE(inFile.is_open());

    std::string line;
    std::getline(inFile, line);
    inFile.close();

    EXPECT_EQ(line, "5 0xBBBB1111");
}

TEST_F(FileIOFixture, ReadNandFile_ValueExists) {

    std::ofstream out("ssd_nand.txt");
    out << "7 0xABCD1234\n";
    out.close();

    pc = { "R", 7, "", false };
    file_io.ReadNandFile(pc);

    std::ifstream inFile("ssd_output.txt");
    ASSERT_TRUE(inFile.is_open());

    std::string line;
    std::getline(inFile, line);
    inFile.close();

    EXPECT_EQ(line, "0xABCD1234");
}

TEST_F(FileIOFixture, ReadNandFile_ValueNotFound) {
    std::remove("ssd_nand.txt");
    pc = { "R", 99, "", false };
    file_io.ReadNandFile(pc);

    std::ifstream inFile("ssd_output.txt");
    ASSERT_TRUE(inFile.is_open());

    std::string line;
    std::getline(inFile, line);
    inFile.close();

    EXPECT_EQ(line, "0x00000000");
}

TEST_F(FileIOFixture, ReadNandFile_WithErrorFlag) {
    pc = { "R", 10, "", true };
    file_io.ReadNandFile(pc);

    std::ifstream inFile("ssd_output.txt");
    ASSERT_TRUE(inFile.is_open());

    std::string line;
    std::getline(inFile, line);
    inFile.close();

    EXPECT_EQ(line, "ERROR");
}