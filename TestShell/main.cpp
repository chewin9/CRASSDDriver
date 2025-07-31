#include <gmock/gmock.h>
#include "shell.h"

int main(int argc, char* argv[]) {
#if (_DEBUG)
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else 
	Shell shell;
	return shell.Run((argc == 1) ? "" : argv[1]);
#endif 
}
