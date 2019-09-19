#include "Globals.hpp"
#include "WarbandServer.hpp"
//#include "ServerQuery.hpp"

#include <iostream> //

//extern ServerQuery *gServerQuery;

WarbandServer::WarbandServer(QObject *parent) : QObject(parent)
{

}

void WarbandServer::setMainWindow(MainWindow *mainWindow)
{
	this->mainWindow = mainWindow;
}

void WarbandServer::addPlayer(const Player *player)
{
	std::cout << "Player Joined !: " << player->uid << std::endl;
	//this->mainWindow->addPlayer(player);
	emit newPlayerEvent(player);
}

void WarbandServer::removePlayer(unsigned uid)
{
	std::cout << "Player Left: " << uid << std::endl;
	//this->mainWindow->removePlayer(uid);
	emit removePlayerEvent(uid);
}

void WarbandServer::kickPlayer(unsigned uid) noexcept
{
	ServerQuery::MsgPlayerKick msgKick;

	msgKick.uid = uid;
	gServerQuery->sendMessage(ServerQuery::MessageType::GAME_KICK_PLAYER, (BYTE *) &msgKick, sizeof(msgKick));
}
