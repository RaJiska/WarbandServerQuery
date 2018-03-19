#include "WarbandServerQuery.hpp"
#include "Hooks.hpp"
#include "Addresses.hpp"

void hookConnect_PlayerJoined(void)
{
	
}

__declspec(naked) void hook_PlayerJoined(void)
{
	__asm
	{
		FREEZE_REGS
		mov Addresses::playerJoined_playerBase, esi
		call hookConnect_PlayerJoined
		RESTORE_REGS
		cmp [esi + 0F070h], 2
		jmp [Addresses::playerJoined_ExitPoint]
	}
}