#include "gmock/gmock.h"
#include "file_io.h"
#include "command_parser.h"
#include "readcommand.h"
#include "command_factory.h"
#include "writecommand.h"
#include "ssd_operation_handler.h"

TEST(CommandFactoryTest, CreateReadCommand) { 
  CommandFactory factory;
  ParsedCommand cmd = {"R", 10, "", false};
  FileIO fileio;
  CommandBuffer cmdbuffer{ nullptr };
  SsdOperationHandler opHandler(fileio, cmdbuffer);
  std::unique_ptr<ICommand> result = CommandFactory::create(cmd.opCode, opHandler);


  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(dynamic_cast<ReadCommand*>(result.get()) != nullptr);


}

TEST(CommandFactoryTest, CreateWriteCommand) {
  CommandFactory factory;
  ParsedCommand cmd = {"W", 10, "0xAAAABBBB", false};
  FileIO fileio;
  CommandBuffer cmdbuffer(std::move(std::make_unique<WriteCommandOptimizer>()));
  SsdOperationHandler opHandler(fileio, cmdbuffer);
  std::unique_ptr<ICommand> result = factory.create(cmd.opCode, opHandler);

  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(dynamic_cast<WriteCommand*>(result.get()) != nullptr);
}

TEST(CommandFactoryTest, ReturnNullPtr) {
  CommandFactory factory;
  ParsedCommand cmd = {"S", 10, "0xAAAABBBB", false};
  FileIO fileio;
  CommandBuffer cmdbuffer{ nullptr };;
  SsdOperationHandler opHandler(fileio, cmdbuffer);
  std::unique_ptr<ICommand> result = factory.create(cmd.opCode, opHandler);
  EXPECT_EQ(result, nullptr);

}