#include "file_io.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <filesystem>

#include "command_parser.h"
#include "gmock/gmock.h"
#include "ssd_operation_handler.h"

using namespace ::testing;
namespace fs = std::filesystem;

class FileIOFixture : public Test {
 public:
  FileIO file_io;
  ParsedCommand pc;

  void SetUp() override {
      std::error_code ec;
      fs::remove_all("buffer", ec);
  }
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
  SsdOperationHandler opHandler(file_io);
  opHandler.Write(pc);
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
  pc = {"W", 3, "0xABCDEF01", false};
  SsdOperationHandler opHandler(file_io);
  opHandler.Write(pc);

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

  pc = {"W", 5, "0xBBBB1111", false};
  SsdOperationHandler opHandler(file_io);
  opHandler.Write(pc);

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

  pc = {"R", 7, "", false};
  SsdOperationHandler opHandler(file_io);
  opHandler.Read(pc);

  std::ifstream inFile("ssd_output.txt");
  ASSERT_TRUE(inFile.is_open());

  std::string line;
  std::getline(inFile, line);
  inFile.close();

  EXPECT_EQ(line, "0xABCD1234");
}

TEST_F(FileIOFixture, ReadNandFile_ValueNotFound) {
  std::remove("ssd_nand.txt");
  pc = {"R", 99, "", false};
  SsdOperationHandler opHandler(file_io);
  opHandler.Read(pc);

  std::ifstream inFile("ssd_output.txt");
  ASSERT_TRUE(inFile.is_open());

  std::string line;
  std::getline(inFile, line);
  inFile.close();

  EXPECT_EQ(line, "0x00000000");
}

TEST_F(FileIOFixture, ReadNandFile_WithErrorFlag) {
  pc = {"R", 10, "", true};
  SsdOperationHandler opHandler(file_io);
  opHandler.Read(pc);

  std::ifstream inFile("ssd_output.txt");
  ASSERT_TRUE(inFile.is_open());

  std::string line;
  std::getline(inFile, line);
  inFile.close();

  EXPECT_EQ(line, "ERROR");
}

TEST_F(FileIOFixture, GenBufferFolder) {
    file_io.GenFolderAndEmtyFiles();
    std::ifstream inFile("buffer/1_empty");
    ASSERT_TRUE(inFile.is_open());

    file_io.EraseFolder();
    file_io.GenFolderAndEmtyFiles();
    std::ifstream inFile2("buffer/2_empty");
    ASSERT_TRUE(inFile2.is_open());

}


TEST_F(FileIOFixture, GenEmptyFileAndCheckSizeAndName) {

    file_io.GenFolderAndEmtyFiles();

    auto filenames = file_io.LoadCommandBuffer();
    EXPECT_EQ(filenames.size(), 5u);

    for (int i = 1; i <= 5; ++i) {
        const std::string name = std::to_string(i) + "_empty";
        EXPECT_NE(std::find(filenames.begin(), filenames.end(), name), filenames.end()
        );

        EXPECT_EQ(filenames[i-1], name);
    }
}


TEST_F(FileIOFixture, CheckChangedFileName) {

    file_io.GenFolderAndEmtyFiles();
    std::vector<std::string> in_command = { "1_ABC", "2_DEF", "3_HIJ" };

    file_io.ChangeFileName(in_command);

}