#include "HookManager.hpp"
#include "WarbandServer.hpp"
#include "Hook.hpp"

#include <iostream>

HookManager::HookManager(bool setupHooks)
{
	if (setupHooks)
		this->setupHooks();
}

void HookManager::setupHooks(void)
{
	this->placeHook(
		(BYTE *) WarbandServer::Addresses::playerJoined_EntryPoint,
		(DWORD) Hook::playerJoined,
		WarbandServer::Addresses::playerJoined_ExitPoint - WarbandServer::Addresses::playerJoined_EntryPoint
	);
	//hookMngr.placeHook((BYTE*) Addresses::chatMessageSent_EntryPoint, (DWORD) hook_ChatMessageSent, Addresses::chatMessageSent_ExitPoint - Addresses::chatMessageSent_EntryPoint);
}

void HookManager::placeHook(BYTE *address, DWORD jumpTo, SIZE_T len)
{
	DWORD oldProtect;
	HookBackup bak;

	bak.address = address;
	bak.data = new BYTE [len];
	bak.len = len;
	VirtualProtect(address, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	std::memcpy(&bak.data[0], address, len);
	*address = 0xE9;
	*((DWORD *) (address + 0x1)) = (DWORD) (jumpTo - (DWORD) address) - 5;
	for (SIZE_T i = 0x5; i < len; ++i)
		*(address + i) = 0x90;
	VirtualProtect(address, len, oldProtect, &oldProtect);
	this->oldHookData.push_back(bak);
}

void HookManager::revertHooks(void)
{
	DWORD oldProtect;

	for (auto &it : this->oldHookData) {
		VirtualProtect(it.address, it.len, PAGE_EXECUTE_READWRITE, &oldProtect);
		std::memcpy(it.address, it.data, it.len);
		VirtualProtect(it.address, it.len, oldProtect, &oldProtect);
		delete it.data;
	}
	this->oldHookData.clear();
}