#include "gmock/gmock.h"
#include "easylogging++.h"
#include <limits.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) 
{
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS(); 
}
