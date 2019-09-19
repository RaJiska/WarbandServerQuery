#include "ConnectWindow.hpp"
#include "Globals.hpp"
#include "ui_connectwindow.h"
#include "MainWindow.hpp"

#include <QtConcurrent/QtConcurrent>

ConnectWindow::ConnectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectWindow),
    wParent(parent)
{
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	ui->setupUi(this);
	QObject::connect(ui->but_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
	QObject::connect(ui->but_connect, SIGNAL(clicked(bool)), this, SLOT(connectToServer()));
}

void ConnectWindow::connectToServer(void)
{
	gServerQuery->setAddress(ui->le_host->text().toStdString());
	gServerQuery->setPort(ui->le_port->text().toStdString());
	gServerQuery->setPassword(ui->le_passwd->text().toStdString());
	//QFuture<void> future = QtConcurrent::run(gServerQuery, &ServerQuery::connect);
	this->future = QtConcurrent::run(gServerQuery, &ServerQuery::connect);
}
