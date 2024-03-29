#pragma once

#include "windows.h"

class WarbandServer
{
public:
	const unsigned targetVersion = 1174;

	struct Addresses
	{
		const static unsigned playerJoined_EntryPoint = 0x0042741E;
		const static unsigned playerJoined_ExitPoint = 0x00427463;
		const static unsigned playerLeft_EntryPoint = 0x004384AF;
		const static unsigned playerLeft_ExitPoint = 0x004384B4;
		const static unsigned chatMessageSent_EntryPoint = 0x00439A90;
		const static unsigned chatMessageSent_ExitPoint = 0x00439A96;
		const static unsigned logEntryAdded_EntryPoint = 0x00484B40;
		const static unsigned logEntryAdded_ExitPoint = 0x00484B46;

		const static unsigned fncBanPlayer = 0x0043C760;
		const static unsigned fncSendMessage = 0x00439A90;
		const static unsigned fncKickPlayer = 0x00427560;
		const static unsigned fncKickPlayerThis = 0x00ABF158;
	};

	// 40840h
	typedef struct
	{
		/* Pre-Header at 0x130 above */
		BYTE unk1[0x38];
		UINT32 uniqueId; // 0x38
		BYTE unk2[0x1C];
		BYTE ipAddress[4]; // 0x58
		UINT16 port; // 5C
		BYTE unk3[0x22];
		BYTE id; // 0x80
		BYTE unk4[0xEFEF];
		UINT32 role; // 0xF070
		BYTE unk5[0x10];
		BYTE name[0x1D]; // 0xF084
		BYTE unkEnd[0x317BC];
	} Player;

	// 110h
	typedef struct
	{
		UINT32 unk1;
		UINT32 unk2; // Constant value 02 E0 EB 88
		UINT32 unk3; // Constant value 00 00 00 80
		UINT32 size;
		char message[0x100]; // Size TBC
	} ChatMessage;

	WarbandServer() = default;
	virtual ~WarbandServer() = default;

	virtual void synchronizeNewClient(unsigned long long clientId);
	virtual void playerJoined(Player *player);
	virtual void playerLeft(unsigned uniqueId);
	virtual void kickPlayer(unsigned playerUid);
private:
	std::vector<Player *> players;
};

