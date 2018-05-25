#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include "win-basic-main.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QLabel label("Hello World");
	label.show(); 

    return a.exec(); 
}