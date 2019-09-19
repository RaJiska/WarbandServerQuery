#include "MainWindow.hpp"
#include "Globals.hpp"

#include <QApplication>

WarbandServer *gWarbandServer = new WarbandServer();
ServerQuery *gServerQuery = new ServerQuery();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	//QFuture<void> future = QtConcurrent::run(gServerQuery, &ServerQuery::connect);
	w.show();
	return a.exec();
}
