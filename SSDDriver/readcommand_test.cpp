#include "readcommand.h"

#include <string>
#include <vector>

#include "gmock/gmock.h"

using namespace testing;

class ReadCommandFixture : public Test {
 public:
  ReadCommand rc;
  const int MAX_LBA = 100;
  std::string normalValue = "0xFFFFFFFF";
};

TEST_F(ReadCommandFixture, NormalRead) {
  for (int normalLba = 0; normalLba < MAX_LBA; normalLba++) {
    EXPECT_TRUE(rc.Execute(normalLba, normalValue));
  }
}
