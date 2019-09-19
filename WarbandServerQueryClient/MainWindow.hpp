#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Player.hpp"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void setupPlayerContextMenu(const QPoint &pos);
	void addPlayer(const Player *player);
	void removePlayer(unsigned uid);
	void openConnectToServer(void);
	//void DisconnectFromServer();

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
