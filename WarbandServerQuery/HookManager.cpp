#include "HookManager.hpp"

void HookManager::placeHook(BYTE *address, DWORD jumpTo, SIZE_T len)
{
	DWORD oldProtect;
	hookBackup_t bak;

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

	for (auto &it : this->oldHookData)
	{
		VirtualProtect(it.address, it.len, PAGE_EXECUTE_READWRITE, &oldProtect);
		std::memcpy(it.address, it.data, it.len);
		VirtualProtect(it.address, it.len, oldProtect, &oldProtect);
		delete it.data;
	}
	this->oldHookData.clear();
}