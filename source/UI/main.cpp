#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include "win-basic-main.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	MEBasic basic; 
	basic.show(); 
    a.exec(); 
    return 0; 
}