#include <iostream>
#include <vector>
#include <chrono>

using namespace std; 
using namespace std::chrono; 
           
int main(int argc, char* argv[]) {
	steady_clock::duration d = steady_clock::now().time_since_epoch();
	minutes min = duration_cast<minutes>(d);
	seconds sec = duration_cast<seconds>(d);
	milliseconds mil = duration_cast<milliseconds>(d);
	microseconds mic = duration_cast<microseconds>(d);
	nanoseconds nan = duration_cast<nanoseconds>(d);
	cout << min.count() << "����" << endl;
	cout << sec.count() << "��" << endl;
	cout << mil.count() << "����" << endl;
	cout << mic.count() << "΢��" << endl;
	cout << nan.count() << "����" << endl;

	return 0; 
}