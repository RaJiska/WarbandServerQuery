#include "WarbandServerQueryAdmin.hpp"
#include "WarbandServer.hpp"

#include <iostream>

void WarbandServer::playerJoined(Player *player)
{
	ControlServer::MsgPlayerJoined msg;

	msg.id = 0;
	msg.uid = (player->uniqueId & 0x00FFFFFF);
	std::memcpy(&msg.ipAddress[0], &player->ipAddress[0], 4);
	msg.port = player->port;
	std::strncpy(reinterpret_cast<char*>(msg.name), reinterpret_cast<char*>(&player->name[0]), 29);
	msg.role = player->role;
	gControlServer->broadcastMessage(
		ControlServer::GAME_PLAYER_JOINED,
		reinterpret_cast<BYTE *>(&msg),
		sizeof(ControlServer::MsgPlayerJoined)
	);
	this->players.push_back(player);
}

void WarbandServer::playerLeft(unsigned uniqueId)
{
	ControlServer::MsgPlayerLeft msg;

	msg.uid = uniqueId;
	gControlServer->broadcastMessage(
		ControlServer::GAME_PLAYER_LEFT,
		reinterpret_cast<BYTE *>(&msg),
		sizeof(ControlServer::MsgPlayerLeft)
	);
	for (auto it = this->players.begin(); it != this->players.end(); ++it) {
		if ((*it)->uniqueId == uniqueId) {
			this->players.erase(it);
			break;
		}
	}
}

void WarbandServer::synchronizeNewClient(unsigned long long clientId)
{
	ControlServer::MsgPlayerJoined msg;

	for (auto &it : this->players) {
		msg.id = 0;
		msg.uid = (it->uniqueId & 0x00FFFFFF);
		std::memcpy(&msg.ipAddress[0], &it->ipAddress[0], 4);
		std::strncpy(reinterpret_cast<char *>(&msg.name[0]), reinterpret_cast<char *>(&it->name[0]), 29);
		msg.role = it->role;
		std::cout << "Sending: Player Name: " << msg.name << std::endl;
		gControlServer->sendMessage(
			clientId,
			ControlServer::GAME_PLAYER_JOINED,
			reinterpret_cast<BYTE *>(&msg),
			sizeof(ControlServer::MsgPlayerJoined)
		);
	}
}

void WarbandServer::kickPlayer(unsigned uniqueId)
{
	int index = 0x0;
	void (__thiscall *kickPlayer)(void *pThis, int *index) = (void (__thiscall *)(void *, int *)) WarbandServer::Addresses::fncKickPlayer;

	std::cout << "Kick Player: " << uniqueId << std::endl;
	for (auto it : this->players) {
		if ((it->uniqueId & 0x00FFFFFF) == uniqueId) {
			index = (((BYTE *) it - (BYTE *) WarbandServer::Addresses::fncKickPlayerThis) / 0x10210);
			break;
		}
	}
	if (index)
		kickPlayer((void*)WarbandServer::Addresses::fncKickPlayerThis, &index);
}