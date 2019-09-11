#include "HookManager.hpp"

#include <iostream>

void SetupDebugConsole()
{
	AllocConsole();
	freopen("conin$", "r+t", stdin);
	freopen("conout$", "w+t", stdout);
	freopen("conout$", "w+t", stderr);
	std::cout << "Console started !" << std::endl;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	static HookManager manager(false);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetupDebugConsole();
		manager.setupHooks();
		break;
	case DLL_PROCESS_DETACH:
		manager.revertHooks();
		break;
	}
	return TRUE;
}

