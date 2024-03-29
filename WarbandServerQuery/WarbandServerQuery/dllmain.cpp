#include "WarbandServerQueryAdmin.hpp"
#include "HookManager.hpp"

#include <iostream>

WarbandServer *gWarbandServer = new WarbandServer();
ControlServer *gControlServer = new ControlServer(8888);

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
		gControlServer->run();
		break;
	case DLL_PROCESS_DETACH:
		manager.revertHooks();
		break;
	}
	return TRUE;
}

