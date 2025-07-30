#include "readcommand.h"

#include <string>
#include <vector>

#include "gmock/gmock.h"

using namespace testing;

class ReadCommandFixture : public Test {
 public:

  const int MAX_LBA = 100;
  std::string normalValue = "0xFFFFFFFF";
};

TEST_F(ReadCommandFixture, NormalRead) {
  for (int normalLba = 0; normalLba < MAX_LBA; normalLba++) {
    ParsedCommand cmdInfo = {"R", normalLba};
    ReadCommand rc(cmdInfo);
    EXPECT_TRUE(rc.Execute());
  }
}
