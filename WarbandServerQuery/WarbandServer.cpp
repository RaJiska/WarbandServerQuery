#include "WarbandServerQueryAdmin.hpp"
#include "WarbandServer.hpp"

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