#include "readcommand.h"

#include <string>

#include "gmock/gmock.h"

TEST(ReadCommand, Read) {
  ReadCommand rc;
  int lba = 10;
  std::string value = "0xFFFFFFFF";
  EXPECT_TRUE(rc.Read(lba, value));
}