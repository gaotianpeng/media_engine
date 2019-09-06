#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include "win-basic-main.h"
#include "mymath.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	print_test(); 
	QLabel label("Hello World");
	label.show(); 

    return a.exec(); 
}