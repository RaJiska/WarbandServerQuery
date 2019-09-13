#include "WarbandServerQuery.hpp"
#include "Hooks.hpp"
#include "Addresses.hpp"

void __stdcall hookConnect_ChatMessageSent(const char *msg)
{
	std::cout << "Coucou: " << msg << std::endl;
}

__declspec(naked) void hook_ChatMessageSent(void)
{
	__asm
	{
		FREEZE_REGS
		add eax, 10h
		push eax
		call hookConnect_ChatMessageSent
		RESTORE_REGS
		push eax
		mov ecx, 0AB5C60h
		jmp [Addresses::chatMessageSent_ExitPoint]
	}
}