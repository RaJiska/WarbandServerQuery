#include "WarbandServerQueryAdmin.hpp"
#include "Hook.hpp"

#include <iostream>
#include <fstream>

extern "C" static void __stdcall connectPlayerJoined(WarbandServer::Player *player)
{
	if (player->uniqueId == 0)
		return;
	gWarbandServer->playerJoined(player);
	int id = *((int *) player);
	printf("Addr: %X\n", &player->id);
	std::cout << "(" << id << ") [" << (void *)player <<
		"] [" << (int)player->ipAddress[0] << "." << (int)player->ipAddress[1] << "." <<
		(int)player->ipAddress[2] << "." << (int)player->ipAddress[3] << ":" << player->port <<
		"] Player '" << (const char *)player->name <<
		"' with ID: " << (int)(player->uniqueId & 0x00FFFFFF) << " (" << (int)player->id << ")" <<
		" joined the game with role: " << player->role << std::endl;

}

extern "C" __declspec(naked) void Hook::playerJoined(void)
{
	__asm
	{
		push eax /* Reserve space for future player pointer; adds 0x4 to every ESP */
		push eax
		mov eax, [esp + 0x38 + 0x4]
		push edx
		mov edx, [esp + 0x38 + 0x4]
		push eax
		mov eax, [esp + 0x38 + 0x4]
		push edx
		mov edx, [esp + 0x38 + 0x4]
		push eax
		mov eax, [esp + 0x38 + 0x4]
		push edx
		mov edx, [esp + 0x30 + 0x4]
		push eax
		mov eax, [esp + 0x30 + 0x4]
		push esi
		mov esi, [esp + 0x3C + 0x4]
		push edx
		mov edx, [esp + 0x34 + 0x4]
		push eax
		mov eax, [esi]
		imul eax, 0x10210
		push edx
		lea ecx, [eax + ecx + 0x130]
		mov [esp + 0x2C], ecx // Save player pointer to our reserved space
		mov bl, 1
		// Workaround to unsuported direct CALL
		mov eax, 0x00428F80
		call eax
		call connectPlayerJoined // Restore stack of our reserved space on the way
		jmp [WarbandServer::Addresses::playerJoined_ExitPoint]
	}
}

extern "C" static void __stdcall connectPlayerLeft(WarbandServer::Player *player)
{
	gWarbandServer->playerLeft(player->uniqueId & 0x00FFFFFF);
}

extern "C" __declspec(naked) void Hook::playerLeft(void)
{
	__asm
	{
		FREEZE_REGS
		push esi
		call connectPlayerLeft
		RESTORE_REGS
		mov ecx, 0x006E44D8
		jmp [WarbandServer::Addresses::playerLeft_ExitPoint]
	}
}

extern "C" static void __stdcall connectChatMessageSent(WarbandServer::ChatMessage *msg)
{
	/* Do Something */
}

extern "C" __declspec(naked) void Hook::chatMessageSent(void)
{
	__asm
	{
		push ecx
		push [esp + 0x8 + 0x4] // Second param
		call connectChatMessageSent
		pop ecx

		sub esp, 0x350
		jmp [WarbandServer::Addresses::chatMessageSent_ExitPoint]
	}
}

extern "C" static void __stdcall connectLogEntryAdded(const char *msg)
{
	//std::cout << "New Log Entry: " << msg << std::endl;
}

extern "C" __declspec(naked) void Hook::logEntryAdded(void)
{
	__asm
	{
		push ecx
		mov ecx, esp
		add ecx, 0x20
		push ecx
		call connectLogEntryAdded
		pop ecx

		sub esp, 0x110
		jmp [WarbandServer::Addresses::logEntryAdded_ExitPoint]
	}
}
