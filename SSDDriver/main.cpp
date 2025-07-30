#include "gmock/gmock.h"
#include "ssd_driver_config.h"

#if UNITTEST_ENABLE
int main(void) {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[]) { std::cout << argc << std::endl;
  std::cout << argv[0] << ", " << argv[1] << ", " << argv[2] << ", " << argv[3]
            << std::endl;
}
#endif
