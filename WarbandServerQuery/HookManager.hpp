#pragma once
#include "WarbandServerQuery.hpp"
#include <vector>
#include <cstring>

class HookManager
{
	public:
	HookManager() = default;
	virtual ~HookManager() = default;

	void placeHook(BYTE *address, DWORD jumpTo, SIZE_T len);
	void revertHooks(void);

	private:
	typedef struct hookBackUp_s
	{
		BYTE *address;
		BYTE *data;
		SIZE_T len;
	} hookBackup_t;
	std::vector<hookBackup_t> oldHookData;
};