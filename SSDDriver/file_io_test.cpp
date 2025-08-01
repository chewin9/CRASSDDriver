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
      std::remove("ssd_output.txt");
  }

  void TearDown() override {
      std::error_code ec;
      fs::remove_all("buffer", ec);
      std::remove("ssd_output.txt");
  }
};

TEST_F(FileIOFixture, WriteErrorOutput) {
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
  pc.errorFlag = true;
  CommandBuffer cmdbuffer{file_io};
  SsdOperationHandler opHandler(file_io, cmdbuffer);
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

TEST_F(FileIOFixture, WriteOutputWithNewLBA_ThroughReadCheck) {
  std::remove("ssd_nand.txt");

  // Write
  pc = {"W", 3, "0xABCDEF01", false};
  CommandBuffer cmdbuffer{file_io};
  SsdOperationHandler opHandler(file_io, cmdbuffer);
  opHandler.Write(pc);

  // Read to flush buffer and write output
  ParsedCommand readCmd = {"R", 3, "", false};
  opHandler.Read(readCmd);

  // Check output file
  std::ifstream inFile("ssd_output.txt");
  ASSERT_TRUE(inFile.is_open());

  std::string line;
  std::getline(inFile, line);
  inFile.close();

  EXPECT_EQ(line, "0xABCDEF01");
}

TEST_F(FileIOFixture, WriteOutput_OverwritesExistingLBA_ThroughReadCheck) {
  std::remove("ssd_nand.txt");

  // 1. Write initial value
  {
    ParsedCommand initial = {"W", 5, "0xAAAA0000", false};
    CommandBuffer cmdbuffer{file_io};
    SsdOperationHandler opHandler(file_io, cmdbuffer);
    opHandler.Write(initial);

    ParsedCommand readInit = {"R", 5, "", false};
    opHandler.Read(readInit);  // flushes initial write
  }

  // 2. Overwrite with new value
  pc = {"W", 5, "0xBBBB1111", false};
  CommandBuffer cmdbuffer{file_io};
  SsdOperationHandler opHandler(file_io, cmdbuffer);
  opHandler.Write(pc);

  ParsedCommand readCmd = {"R", 5, "", false};
  opHandler.Read(readCmd);  // triggers flush and read output

  std::ifstream inFile("ssd_output.txt");
  ASSERT_TRUE(inFile.is_open());

  std::string line;
  std::getline(inFile, line);
  inFile.close();

  EXPECT_EQ(line, "0xBBBB1111");
}


TEST_F(FileIOFixture, ReadNandFile_ValueExists) {
  std::ofstream out("ssd_nand.txt");
  out << "7 0xABCD1234\n";
  out.close();

  pc = {"R", 7, "", false};
  CommandBuffer cmdbuffer{file_io};
  SsdOperationHandler opHandler(file_io, cmdbuffer);
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
  CommandBuffer cmdbuffer{file_io};
  SsdOperationHandler opHandler(file_io, cmdbuffer);
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
  CommandBuffer cmdbuffer{file_io};
  SsdOperationHandler opHandler(file_io, cmdbuffer);
  opHandler.Read(pc);

  std::ifstream inFile("ssd_output.txt");
  ASSERT_TRUE(inFile.is_open());

  std::string line;
  std::getline(inFile, line);
  inFile.close();

  EXPECT_EQ(line, "ERROR");
}


//TEST_F(FileIOFixture, CheckChangedFileName) {
//    std::vector<std::string> in_command = { "1_ABC", "2_DEF", "3_HIJ" };
//    file_io.ChangeFileName(in_command);
//
//    auto load_command = file_io.getCommandBuffer();
//
//    ASSERT_EQ(load_command.size(), in_command.size());
//
//    for (size_t i = 0; i < in_command.size(); ++i) {
//        EXPECT_EQ(load_command[i], in_command[i]);
//    }
//}
//
//TEST_F(FileIOFixture, CheckChangedFileName) {
//    std::vector<std::string> in_command = { "1_ABC", "2_DEF", "3_HIJ" };
//    file_io.ChangeFileName(in_command);
//
//    auto load_command = file_io.getCommandBuffer();
//
//    ASSERT_EQ(load_command.size(), in_command.size());
//
//    for (size_t i = 0; i < in_command.size(); ++i) {
//        EXPECT_EQ(load_command[i], in_command[i]);
//    }
//}