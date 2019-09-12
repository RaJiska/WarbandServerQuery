#include "WarbandServerQueryAdmin.hpp"
#include "Hook.hpp"

#include <iostream>

extern "C" static void __stdcall connectPlayerJoined(WarbandServer::Player *player)
{
	gWarbandServer.playerJoined(player);
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
		jmp [WarbandServer::Addresses::playerJoined_ExitPoint]
	}
}

extern "C" static void __stdcall connectChatMessageSent(WarbandServer::ChatMessage *msg)
{
	std::cout << "Pushed Addr: " << (void*)msg << " ; " << &msg->message << " ; " << &msg->size<< std::endl;
	std::cout << "Message Sent: " << msg->message << " [" << msg->size << "]" << std::endl;
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
	std::cout << "New Log Entry: " << msg << std::endl;
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
