#pragma once

#include <Windows.h>

class WarbandServer
{
public:
	const unsigned targetVersion = 1174;

	struct Addresses
	{
		const static unsigned playerJoined_EntryPoint = 0x0042916C;
		const static unsigned playerJoined_ExitPoint = 0x00429171;

		const static unsigned fncBanPlayer = 0x0043C760;
		const static unsigned fncSendMessage = 0x00439A90;
	};

	// 40840h
	typedef struct
	{
		BYTE unk1[0x37];
		UINT32 uniqueId; // 0x38
		BYTE unk2[0x784A];
		BYTE name[0x1D /* 29 */]; // Size TBC; 0x7886
		BYTE unk3[0x77CD];
		UINT32 role; // 0xF070
		BYTE unkEnd[0x317CB];
	} Player;

	// 10h + MsgSize
	typedef struct
	{
		BYTE unk1[0x6];
		UINT32 unk2; // Constant value 02 E0 EB 88
		UINT32 unk3; // Constant value 00 00 00 80
		UINT32 size;
		char message[0x100]; // Size TBC
	} ChatMessage;

	WarbandServer() = default;
	virtual ~WarbandServer() = default;
};

