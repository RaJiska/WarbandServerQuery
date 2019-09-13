#include "WarbandServer.hpp"

#include <iostream> //

WarbandServer::WarbandServer()
{

}

void WarbandServer::addPlayer(const Player &player)
{
	std::cout << "Player Joined !: " << player.uid << std::endl;
	this->players.push_back(player);
}

void WarbandServer::kickPlayer(const Player &player) noexcept
{

}
