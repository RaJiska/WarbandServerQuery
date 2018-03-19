#include "WarbandServerQuery.hpp"

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
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetupDebugConsole();
		WarbandServerQuery();
		break;
	case DLL_PROCESS_DETACH:
		std::cout << "Bye bye !" << std::endl;
		break;
	}
	return TRUE;
}

