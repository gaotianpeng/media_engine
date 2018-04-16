#include "QWindowCourse.h"
#include <QtWidgets/QApplication>
#include "AccountAuth.h"
#include "QDialogProgress.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	QApplication::setQuitLockEnabled(true);
    
	QDialogProgress::instance().initInThread();

    AccountAuth auth(nullptr);
    auth.exec();

    QWindowCourse w;
    if (auth.isEnterRoom())
    {	
		a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(exit()));
        w.init(auth);
        w.show();
    }
	else
	{
		auth.quit();
	}	

    return a.exec();
}
