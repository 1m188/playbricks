#include <QtWidgets/QApplication>
#include "maingui.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainGui *mainGui = new MainGui(nullptr);
	mainGui->show();
	return a.exec();
}
