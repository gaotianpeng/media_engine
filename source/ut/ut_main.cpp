#include <iostream>
#include <gtest/gtest.h>
#include <glog/logging.h>


using namespace std;

int main(int argc, char* argv[]) {
	cout << "hello world" << endl;
	google::InitGoogleLogging(argv[0]);
	::testing::InitGoogleTest(&argc, argv);
	FLAGS_logtostderr = 1;
	LOG(INFO) << "unit test logging ..." ;

	return RUN_ALL_TESTS();
}