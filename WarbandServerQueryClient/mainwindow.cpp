#include "Globals.hpp"
#include "MainWindow.hpp"
#include "ConnectWindow.hpp"
#include "ui_mainwindow.h"

#include <iostream> // DEBUG

/* TODO: With timer refresh content of W */

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QObject::connect(ui->actionServerConnect, SIGNAL(triggered(bool)), this, SLOT(openConnectToServer()));
	QObject::connect(ui->tw_playerList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(setupPlayerContextMenu(const QPoint &)));
	QObject::connect(gWarbandServer, SIGNAL(newPlayerEvent(const Player *)), this, SLOT(addPlayer(const Player *)));
	QObject::connect(gWarbandServer, SIGNAL(removePlayerEvent(unsigned)), this, SLOT(removePlayer(unsigned)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setupPlayerContextMenu(const QPoint &pos)
{
	QMenu *contextualMenu = new QMenu(ui->tw_playerList);
	QPoint globalPos = ui->tw_playerList->mapToGlobal(pos);

	contextualMenu->addAction("Kick");
	contextualMenu->addAction("Do Smthing Else");

	QAction *selectedItem = contextualMenu->exec(globalPos);
	if (selectedItem)
	{
		std::cout << "Selected Item: " << selectedItem->text().toUtf8().constData() << std::endl;
		if (selectedItem->text() == "Kick")
		{
			if (ui->tw_playerList->selectedItems()[0])
				gWarbandServer->kickPlayer(ui->tw_playerList->selectedItems()[0]->text(1).toUInt());
		}
	}
}

void MainWindow::openConnectToServer(void)
{
	ConnectWindow *w = new ConnectWindow(this);

	w->setModal(true);
	w->show();
}

void MainWindow::addPlayer(const Player *player)
{
	QString role;
	QString ipAddress = QString::number(player->ipAddress[0]) + "." +
			QString::number(player->ipAddress[1]) + "." +
			QString::number(player->ipAddress[2]) + "." +
			QString::number(player->ipAddress[3]) + ":" +
			QString::number(player->port);

	switch (player->role) {
	case 1:
		role = "Regular";
		break;
	case 2:
		role = "Premium";
		break;
	case 3:
		role = "Admin";
		break;
	}
	QTreeWidgetItem *widgetItem = new QTreeWidgetItem(ui->tw_playerList);
	widgetItem->setText(0, QString::number(player->id));
	widgetItem->setText(1, QString::number(player->uid));
	widgetItem->setText(2, QString::fromStdString(player->name));
	widgetItem->setText(3, role);
	widgetItem->setText(4, ipAddress);
	ui->tw_playerList->insertTopLevelItem(0, widgetItem);
}

void MainWindow::removePlayer(unsigned uid)
{
	QTreeWidgetItemIterator it(ui->tw_playerList);

	while (*it) {
		if ((*it)->text(1) == QString::number(uid))
			delete (*it);
		it++;
	}
}
