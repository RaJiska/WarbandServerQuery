#ifndef WARBANDSERVER_HPP
#define WARBANDSERVER_HPP

#include "Player.hpp"
#include "MainWindow.hpp"

#include <QObject>
#include <vector>

class WarbandServer : public QObject
{
	Q_OBJECT
public:
	WarbandServer(QObject *parent = 0);
	virtual ~WarbandServer() = default;

	virtual void setMainWindow(MainWindow *mainWindow);
	virtual void addPlayer(const Player *player);
	virtual void removePlayer(unsigned uid);
	virtual void kickPlayer(unsigned uid) noexcept;

signals:
	void newPlayerEvent(const Player *player);
	void removePlayerEvent(unsigned uid);

private:
	std::vector<Player> players;
	MainWindow *mainWindow;
};

#endif // WARBANDSERVER_HPP
