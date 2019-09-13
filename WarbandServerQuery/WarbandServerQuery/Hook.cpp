#include "WarbandServerQueryAdmin.hpp"
#include "Hook.hpp"

#include <iostream>
#include <fstream>

extern "C" static void __stdcall connectPlayerJoined(WarbandServer::Player *player)
{
	/*
	UINT32* tmp = (UINT32 *) player;
	std::ofstream ofs = std::ofstream("C:/Users/RaJiska/source/file.txt", std::ofstream::out | std::ofstream::app);
	for (unsigned int it = 0; it < 16384; ++it) {
		ofs << "[" << (void*)tmp << "] " << *tmp << std::endl;
		++tmp;
	}
	ofs << "END END END END END";
	ofs.close();
	*/

	gWarbandServer->playerJoined(player);
	int id = *((int *) player);
	std::cout << "Name Addr: " << (void *) player->name << std::endl;
	std::cout << "(" << id << ") [" << (void*)player <<
		"] Player '" << (const char*)player->name <<
		"' with ID: " << (int)(player->uniqueId & 0x00FFFFFF) <<
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
		call connectPlayerJoined // Free our reserved space on the way
		jmp [WarbandServer::Addresses::playerJoined_ExitPoint]
	}
}

extern "C" static void __stdcall connectChatMessageSent(WarbandServer::ChatMessage *msg)
{
	//std::cout << "Pushed Addr: " << (void*)msg << " ; " << &msg->message << " ; " << &msg->size<< std::endl;
	//std::cout << "Message Sent: " << msg->message << " [" << msg->size << "]" << std::endl;
	gWarbandServer->kickPlayer(679046);
	std::cout << "Kick: " << (679046) << std::endl;
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
