#include "WarbandServerQuery.hpp"
#include "HookManager.hpp"
#include "Addresses.hpp"
#include "Hooks.hpp"

void WarbandServerQuery(void)
{
	HookManager hookMngr;

	hookMngr.placeHook((BYTE*) Addresses::playerJoined_EntryPoint, (DWORD) hook_PlayerJoined, Addresses::playerJoined_ExitPoint - Addresses::playerJoined_EntryPoint);
	hookMngr.placeHook((BYTE*) Addresses::chatMessageSent_EntryPoint, (DWORD) hook_ChatMessageSent, Addresses::chatMessageSent_ExitPoint - Addresses::chatMessageSent_EntryPoint);
}