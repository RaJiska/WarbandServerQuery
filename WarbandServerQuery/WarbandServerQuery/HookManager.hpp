#pragma once

#include <vector>
#include <cstring>
#include <windows.h>

class HookManager
{
	public:
	HookManager(bool setupHoks);
	virtual ~HookManager() = default;

	void setupHooks(void);
	void placeHook(BYTE *address, DWORD jumpTo, SIZE_T len);
	void revertHooks(void);

	private:
	typedef struct
	{
		BYTE *address;
		BYTE *data;
		SIZE_T len;
	} HookBackup;
	std::vector<HookBackup> oldHookData;
};