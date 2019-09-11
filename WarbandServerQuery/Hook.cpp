#include "Hook.hpp"
#include "WarbandServer.hpp"

#include <iostream>

extern "C" static void __stdcall connectPlayerJoined(WarbandServer::Player *player)
{
	//std::cout << "Name Address: " << (void *) &player->role << std::endl;
	std::cout << "[" << (void*)player <<
		"] Player '" << (const char*)player->name <<
		"' with ID: " << (int)(player->uniqueId & 0x00FFFFFF) <<
		" joined the game with role: " << player->role << std::endl;
}

extern "C" __declspec(naked) void Hook::playerJoined(void)
{
	__asm
	{
		FREEZE_REGS
		push esi
		call connectPlayerJoined
		RESTORE_REGS
		push ebp
		lea ecx, [esp + 0x18]
		jmp[WarbandServer::Addresses::playerJoined_ExitPoint]
	}
}