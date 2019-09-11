#include "WarbandServerQuery.hpp"
#include "Hooks.hpp"
#include "Addresses.hpp"
#include "Player.hpp"

void __stdcall hookConnect_PlayerJoined(Player *player)
{
	//std::cout << "Name Address: " << (void *) &player->role << std::endl;
	std::cout << "[" << (void*) player <<
		"] Player '" << (const char *) player->name <<
		"' with ID: " << (int) (player->uniqueId & 0x00FFFFFF) <<
		" joined the game with role: " << player->role << std::endl;
}

__declspec(naked) void hook_PlayerJoined(void)
{
	__asm
	{
		FREEZE_REGS
		push esi
		call hookConnect_PlayerJoined
		RESTORE_REGS
		push ebp
		lea ecx, [esp + 0x18]
		jmp [Addresses::playerJoined_ExitPoint]
	}
}