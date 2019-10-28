#include <iostream>
#include "easylogging.h"

INITIALIZE_EASYLOGGINGPP 
using namespace std; 

int main(int argc, char* argv[]) {
	el::Configurations conf("d:/log_conf.conf");
	// 重新配置一个单一日志记录器
	el::Loggers::reconfigureLogger("default", conf);
	// 用配置文件配置所有的日志记录器
	el::Loggers::reconfigureAllLoggers(conf);
	// 所有的日志记录器都是从文件中配置完成的，可以使用

	LOG(INFO) << "My first info log using default logger";
	return 0;
}