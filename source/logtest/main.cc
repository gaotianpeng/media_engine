#include <iostream>
#include "easylogging.h"

INITIALIZE_EASYLOGGINGPP 
using namespace std; 

int main(int argc, char* argv[]) {
	el::Configurations conf("d:/log_conf.conf");
	// ��������һ����һ��־��¼��
	el::Loggers::reconfigureLogger("default", conf);
	// �������ļ��������е���־��¼��
	el::Loggers::reconfigureAllLoggers(conf);
	// ���е���־��¼�����Ǵ��ļ���������ɵģ�����ʹ��

	LOG(INFO) << "My first info log using default logger";
	return 0;
}