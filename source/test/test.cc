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
	cout << min.count() << "·ÖÖÓ" << endl;
	cout << sec.count() << "Ãë" << endl;
	cout << mil.count() << "ºÁÃë" << endl;
	cout << mic.count() << "Î¢Ãî" << endl;
	cout << nan.count() << "ÄÉÃë" << endl;

	return 0; 
}