#include "file_io.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include "IFileIO.h"
#include "command_parser.h"
#include "gmock/gmock.h"
#include "ssd_operation_handler.h"

using namespace ::testing;
namespace fs = std::filesystem;

class FileIOFixture : public Test {
 public:
  CommandParser parser;
  FileIO file_io;
  ParsedCommand pc;

  void SetUp() override {
    std::error_code ec;
    fs::remove_all(IFileIO::SSD_COMMAND_BUFFER_FOLDER, ec);
    std::remove(IFileIO::SSD_OUTPUT_FILE.c_str());
    std::remove(IFileIO::SSD_NAND_FILE.c_str());
  }

  void TearDown() override {
    std::error_code ec;
    fs::remove_all(IFileIO::SSD_COMMAND_BUFFER_FOLDER, ec);
    std::remove(IFileIO::SSD_OUTPUT_FILE.c_str());
    std::remove(IFileIO::SSD_NAND_FILE.c_str());
  }
};

class FileIOMock : public IFileIO {
 public:
  MOCK_METHOD((std::unordered_map<int, std::string>), LoadDataFromInput, (),
              (override));
  MOCK_METHOD(void, WriteValueToOutputFile, (const std::string& str),
              (override));
  MOCK_METHOD(void, SaveData,
              ((const std::unordered_map<int, std::string>& entries)),
              (override));
  MOCK_METHOD(void, InitBufferDir, (), (override));
  MOCK_METHOD(void, EraseBufferDir, (), (override));
  MOCK_METHOD(void, ChangeFileName,
              (const std::vector<std::string>& in_command), (override));
  MOCK_METHOD((std::vector<std::string>), getCommandBuffer, (), (override));
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
  CommandBuffer cmdbuffer(std::move(std::make_unique<WriteCommandOptimizer>()));
  SsdOperationHandler opHandler(file_io, cmdbuffer);
  opHandler.WriteAndErase(pc);
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
  // Write
  pc = {"W", 3, "0xABCDEF01", false};
  CommandBuffer cmdbuffer(std::move(std::make_unique<WriteCommandOptimizer>()));
  SsdOperationHandler opHandler(file_io, cmdbuffer);
  opHandler.WriteAndErase(pc);

  ParsedCommand readCmd = {"R", 3, "", false};
  opHandler.Read(readCmd);

  std::ifstream inFile("ssd_output.txt");
  ASSERT_TRUE(inFile.is_open());

  std::string line;
  std::getline(inFile, line);
  inFile.close();

  EXPECT_EQ(line, "0xABCDEF01");
}

TEST_F(FileIOFixture, WriteOutput_OverwritesExistingLBA_ThroughReadCheck) {
  // 1. Write initial value
  {
    ParsedCommand initial = {"W", 5, "0xAAAA0000", false};
    CommandBuffer cmdbuffer(std::move(std::make_unique<WriteCommandOptimizer>()));
    SsdOperationHandler opHandler(file_io, cmdbuffer);
    opHandler.WriteAndErase(initial);

    ParsedCommand readInit = {"R", 5, "", false};
    opHandler.Read(readInit);

    // 2. Overwrite with new value
    pc = {"W", 5, "0xBBBB1111", false};
    CommandBuffer cmdbuffer;
    SsdOperationHandler opHandler(file_io, cmdbuffer);
    opHandler.Write(pc);

    ParsedCommand readCmd = {"R", 5, "", false};
    opHandler.Read(readCmd);

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
    CommandBuffer cmdbuffer;
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
    pc = {"R", 99, "", false};
    CommandBuffer cmdbuffer;
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
    CommandBuffer cmdbuffer;
    SsdOperationHandler opHandler(file_io, cmdbuffer);
    opHandler.Read(pc);

    std::ifstream inFile("ssd_output.txt");
    ASSERT_TRUE(inFile.is_open());
    std::string line;
    std::getline(inFile, line);
    inFile.close();

    EXPECT_EQ(line, "ERROR");
  }


  TEST_F(FileIOFixture, CheckChangedFileName) {
    std::vector<std::string> in_command = {"1_ABC", "2_DEF", "3_HIJ"};

    file_io.ChangeFileName(in_command);

    auto load_command = file_io.getCommandBuffer();

    ASSERT_EQ(load_command.size(), in_command.size());

    for (size_t i = 0; i < in_command.size(); ++i) {
      EXPECT_EQ(load_command[i], in_command[i]);
    }
  }

  class FileIOMockTest : public Test {
   protected:
    FileIOMock mock;
    CommandBuffer cmdBuf{mock};
    SsdOperationHandler handler{mock, cmdBuf};
    ParsedCommand pc;
  };

  TEST_F(FileIOMockTest, Write_WithErrorFlag_WritesErrorOnly) {
    pc = {"W", 1, "", true, 0};

    EXPECT_CALL(mock, WriteValueToOutputFile("ERROR")).Times(1);
    EXPECT_CALL(mock, LoadDataFromInput()).Times(0);
    EXPECT_CALL(mock, SaveData(_)).Times(0);

    handler.Write(pc);
  }

  TEST_F(FileIOMockTest, Read_ValueExists_WritesValue) {
    pc = {"R", 7, "", false, 0};

    EXPECT_CALL(mock, LoadDataFromInput())
        .WillOnce(Return(std::unordered_map<int, std::string>{{7, "VAL"}}));
    EXPECT_CALL(mock, WriteValueToOutputFile("VAL")).Times(1);

    handler.Read(pc);
  }

  TEST_F(FileIOMockTest, Read_ValueNotFound_WritesDefault) {
    pc = {"R", 9, "", false, 0};

    EXPECT_CALL(mock, LoadDataFromInput())
        .WillOnce(Return(std::unordered_map<int, std::string>{}));
    EXPECT_CALL(mock, WriteValueToOutputFile("0x00000000")).Times(1);

    handler.Read(pc);
  }

  TEST_F(FileIOMockTest, Read_WithErrorFlag_WritesError) {
    pc = {"R", 5, "", true, 0};

    EXPECT_CALL(mock, WriteValueToOutputFile("ERROR")).Times(1);
    EXPECT_CALL(mock, LoadDataFromInput()).Times(0);

    handler.Read(pc);
  }
