#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
    QLabel* lab = new QLabel(QStringLiteral("Hello world")); 
    lab->show(); 
    
    a.exec(); 
    return 0; 
}