#pragma once

#define FREEZE_REGS pushad
#define RESTORE_REGS popad

namespace Hook
{
	extern "C" void playerJoined(void);
	void chatMessageSent(void);
}