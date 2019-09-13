#include "WarbandServerQueryAdmin.hpp"
#include "WarbandServer.hpp"

#include <iostream>

void WarbandServer::playerJoined(Player *player)
{
	ControlServer::MsgPlayerJoined msg;

	msg.id = 0;
	msg.uid = (player->uniqueId & 0x00FFFFFF);
	std::strncpy(reinterpret_cast<char*>(msg.name), reinterpret_cast<char*>(&player->name[0]), 29);
	msg.role = player->role;
	gControlServer.broadcastMessage(
		ControlServer::GAME_PLAYER_JOINED,
		reinterpret_cast<BYTE *>(&msg),
		sizeof(ControlServer::MsgPlayerJoined)
	);
	this->players.push_back(player);
}

void WarbandServer::synchronizeNewClient(unsigned long long clientId)
{
	ControlServer::MsgPlayerJoined msg;

	for (auto &it : this->players) {
		msg.id = 0;
		msg.uid = (it->uniqueId & 0x00FFFFFF);
		std::strncpy(reinterpret_cast<char*>(msg.name), reinterpret_cast<char*>(&it->name[0]), 29);
		msg.role = it->role;
		gControlServer.sendMessage(
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

	for (auto it : this->players) {
		if ((it->uniqueId & 0x00FFFFFF) == uniqueId) {
			index = (((BYTE *) it - (BYTE *) WarbandServer::Addresses::fncKickPlayerThis) / 0x10210);
			break;
		}
	}
	if (index)
		kickPlayer((void*)WarbandServer::Addresses::fncKickPlayerThis, &index);
}