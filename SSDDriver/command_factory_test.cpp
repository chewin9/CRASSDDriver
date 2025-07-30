#include "gmock/gmock.h"
#include "file_io.h"
#include "command_parser.h"
#include "readcommand.h"
TEST(CommandFactoryTest, CreateReadCommand) { 
	CommandFactory factory;
  ParsedCommand cmd = {"R", 10, "", false};
  FileIO f;
  
  ICommand* result = factory.create(cmd, f);

  ASSERT_NE(result, nullptr);
  EXPECT_TRUE(dynamic_cast<ReadCommand*>(result) != nullptr);

  delete result;
}

TEST(CommandFactoryTest, CreateWriteCommand) {
  CommandFactory factory;
  ParsedCommand cmd = {"W", 10, "0xAAAABBBB", false};
  FileIO f;

  ICommand* result = factory.create(cmd, f);

  ASSERT_NE(result, nullptr);
  EXPECT_TRUE(dynamic_cast<WriteCommand*>(result) != nullptr);

  delete result;
}

TEST(CommandFactoryTest, ReturnNullPtr) {
  CommandFactory factory;
  ParsedCommand cmd = {"S", 10, "0xAAAABBBB", false};
  FileIO f;

  ICommand* result = factory.create(cmd, f);

  ASSERT_NE(result, nullptr);

  delete result;
}